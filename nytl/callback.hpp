// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the Callback template class.

#pragma once

#ifndef NYTL_INCLUDE_CALLBACK
#define NYTL_INCLUDE_CALLBACK

#include <nytl/connection.hpp> // nytl::BasicConnection
#include <nytl/nonCopyable.hpp> // nytl::NonCopyable
#include <nytl/scope.hpp> // nytl::ScopeGuard

#include <functional> // std::function
#include <forward_list> // std::forward_list
#include <utility> // std::move
#include <cstdint> // std::uint64_t
#include <cstddef> // std::size_t
#include <type_traits> // std::is_same
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include <iostream> // std::cerr
#include <stdexcept> // std::logic_error

namespace nytl {

// TODO (C++17): use std::pmr for more efficient memory allocations (?)

/// List of callback functions.
/// Everyone can register their functions in a callback object and
/// then all registered functions together might be called.
/// The special part about Callback is that it is written for
/// recursive scenarios, e.g. where the callback is called inside a
/// function that was called because the callback was called.
/// So the callback might be accessed from inside such a function.
/// The class is not thread-safe in any way.
/// Exceptions are usually propagated.
/// The class can not be copied and moving it naturally invalidates all
/// Connection objects referencing it (while the connection id objects stay valid).
///
/// \tparam ID A type that fulfills the CallbackConnectionID concept (see docs/callback.md).
/// Examples are ConnectionID or TrackedConnectionID (in nytl/connection.hpp).
template<typename Signature, typename ID = ConnectionID> class Callback;

/// Callback class typedef using TrackedConnectionID, that enables connection to see
/// when the function is unregistered by another connection of because the callback is destroyed.
template<typename Signature> using TrackedCallback = Callback<Signature, TrackedConnectionID>;

// Callback specialization to enable the Ret(Args...) Signature format.
template<typename Ret, typename... Args, typename ID>
class Callback<Ret(Args...), ID> : public ConnectableT<ID>, public NonCopyable {
public:
	using Signature = Ret(Args...);
	using Connection = ConnectionT<ConnectableT<ID>, ID>;

	Callback() = default;

	~Callback()
	{
		// Output warnings in bad cases.
		if(iterationCount_)
			std::cerr << "nytl::~Callback: iterationCount_ != 0\n";

		// We clear manually instead of calling clear so we can at least
		// try to reset every id in the case of exceptions.
		++iterationCount_;
		for(auto& sub : subs_) {
			try {
				sub.id.remove();
			} catch(const std::exception& err) {
				std::cerr << "nytl::~Callback: id.reset() failed: " << err.what() << "\n";
			}
		}
	}

	/// \brief Registers a new function in the same way add does.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::logic_error If an empty function target is registered.
	template<typename F>
	Connection operator+=(F&& func)
	{
		return add(std::forward<F>(func));
	}

	/// \brief Resets all registered function and sets the given one as only Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::logic_error If an empty function target is registered.
	/// Propagates exceptions from ID::reset().
	template<typename F>
	Connection operator=(F&& func)
	{
		clear();
		return add(std::forward<F>(func));
	}

	/// \brief Registers a new Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::logic_error If an empty function target is registered.
	Connection add(std::function<Ret(Args...)> func)
	{
		if(!func)
			throw std::logic_error("nytl::Callback::add: empty function");

		auto& sub = emplace();
		sub.func = std::move(func);
		return {*this, sub.id};
	}

	/// \brief Registers a new Callback function with additional connection parameter.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::logic_error If an empty function target is registered.
	Connection add(std::function<Ret(Connection, Args...)> func)
	{
		if(!func)
			throw std::logic_error("nytl::Callback::add: empty function");

		auto& sub = emplace();
		Connection conn {*this, sub.id};
		sub.func = [conn, f = std::move(func)](Args... args) {
			return f(conn, std::forward<Args>(args)...);
		};

		return conn;
	}

	/// Calls all registered functions and returns a vector with the returned objects,
	/// or void when this is a void callback.
	/// Will call all the functions registered at the moment of calling, i.e. additional
	/// functions added from within are not called.
	/// Propagates all upcoming exceptions untouched.
	auto call(Args... a)
	{
		// wrap callID_ if needed
		callID_ = (callID_ == std::numeric_limits<std::int64_t>::max()) ? 0 : callID_ + 1;
		std::int64_t callid = callID_; // the actual calling id (to include newly removed)
		auto last = end_; // the end (to not iterate over newly added subs)

		// make sure the no list iterators are invalidated while iterating
		++iterationCount_;

		// make sure the iteration count and cleanup done if possible
		// even in the case of an exception.
		auto successGuard = SuccessGuard([&]{
			if(--iterationCount_ == 0)
				removeOld();
		});

		// make sure we catch a potential exception by removeOld if we are leaving
		// the scope because of an exception.
		auto exceptionGuard = ExceptionGuard([&]{
			if(--iterationCount_ == 0) {
				try {
					removeOld();
				} catch(const std::exception& err) {
					std::cerr << "nytl::Callback::call removeOld: " << err.what() << "\n";
				}
			}
		});

		// the first continue check is needed to not call functions that were
		// removed before this call started but call functions that were removed
		// only by some other callback function
		if constexpr(std::is_same<Ret, void>::value) {
			for(auto it = subs_.begin(); it != subs_.end(); ++it) {
				if(it->id.id() <= 0 && -it->id.id() < callid) continue;
				it->func(std::forward<Args>(a)...);
				if(it == last) break;
			}

		} else {
			std::vector<Ret> ret;
			ret.reserve(size());

			for(auto it = subs_.begin(); it != subs_.end(); ++it) {
				if(it->id.id() <= 0 && it->id.id() > callid) continue;
				ret.push_back(it->func(std::forward<Args>(a)...));
				if(it == last) break;
			}

			return ret;
		}
	}

	/// Clears all registered functions.
	/// Will propagate exceptions from ID::reset() or ID::remove().
	void clear()
	{
		{
			// make sure that the iterator is not invalidated while iterating
			++iterationCount_;
			auto scopeGuard = ScopeGuard([&]{ --iterationCount_; });

			// reset the ids or notify the ids of removal
			for(auto& sub : subs_) {
				if(iterationCount_ == 1) sub.id.remove();
				else sub.id.reset(-callID_);
			}
		}

		// clear/remove only if no one is currently iterating
		// we only reset end_ (not really needed, cleanup for easier debugging)
		if(iterationCount_ == 0) {
			subs_.clear();
			end_ = subs_.end();
		}
	}

	/// Operator version of call.
	auto operator() (Args... a)
	{
		return call(std::forward<Args>(a)...);
	}

	/// Removes the callback function registered with the given id.
	/// Returns whether the function could be found. If the id is invalid or the
	/// associated function was already removed, returns false.
	/// Propagates exceptions from ID::reset().
	bool disconnect(const ID& id) override
	{
		if(subs_.empty()) return false;

		// check for first one
		if(subs_.begin()->id == id) {
			if(iterationCount_ == 0) {
				subs_.pop_front();
				subs_.begin()->id.remove();
			} else {
				subs_.begin()->id.reset(-callID_);
			}

			return true;
		}

		// make sure that the iterator is not invalidated while iterating
		++iterationCount_;
		auto scopeGuard = ScopeGuard([&]{ --iterationCount_; });

		// iterate through subs, always check the next elem (fwd linked list)
		// the end condition constructs a copy of it and increases it to check
		for(auto it = subs_.begin(); true; ++it) {
			auto next = it;
			if(++next == subs_.end())
				break;

			if(next->id == id) {
				if(iterationCount_ == 1) {
					next->id.remove();
					subs_.erase_after(it);
				} else {
					next->id.reset(-callID_);
				}

				return true;
			}
		}

		return false;
	}

protected:
	// Represents one callback subscription entry.
	// Invalid (formally removed) when id is not valid.
	// Note that these means that alternative ID classes can
	// remove subscriptions from the outside without actively
	// calling disconnect.
	// We cannot touch func while any iteration is active.
	// If the
	struct Subscription {
		std::function<Ret(Args...)> func;
		ID id;
	};

	// Emplaces a new subscription for the given function.
	Subscription& emplace()
	{
		// emplace at the last position
		if(subs_.empty()) {
			subs_.emplace_front();
			end_ = subs_.begin();
		} else {
			end_ = subs_.emplace_after(end_);
		}

		// output at least a warning when subID_ has to be wrapped
		// Usually this should not happen. Bad things can happend then.
		if(subID_ == std::numeric_limits<std::int64_t>::max()) {
			std::cerr << "nytl::Callback::emplace: WARNING wrapping subID_ at maximum\n";
			subID_ = 0;
		}

		end_->id = {static_cast<std::int64_t>(++subID_)};
		return *end_;
	}

	// Removes all old functions that could previously
	// not be removed because of an active iteration.
	void removeOld()
	{
		// make sure that the iterator is not invalidated while iterating
		++iterationCount_;
		auto scopeGuard = ScopeGuard([&]{ --iterationCount_; });

		subs_.remove_if([](const auto& sub){
			auto remove = sub.id.id() <= 0;
			if(remove) sub.id.remove(); // notify the id
			return remove;
		});
	}

	// Upper approximation of the current size.
	// May be larger than the actual size.
	std::size_t size() const
	{
		std::size_t ret = 0u;
		for(auto it = subs_.begin(); it != subs_.end(); ++it)
			++ret;
		return ret;
	}

	std::forward_list<Subscription> subs_ {}; // all registered subscriptions
	typename decltype(subs_)::iterator end_ {}; // the last entry in subs
	unsigned int iterationCount_ {}; // the number of active iterations (in call)
	std::uint64_t subID_ {}; // the highest subscription id given
	std::uint64_t callID_ {}; // the highest call id given (see the call function)
};

} // namespace nytl

#endif // header guard
