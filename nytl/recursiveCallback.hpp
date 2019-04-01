// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the RecursiveCallback template class.

#pragma once

#ifndef NYTL_INCLUDE_RECURSIVE_CALLBACK
#define NYTL_INCLUDE_RECURSIVE_CALLBACK

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
/// The special part about RecursiveCallback is that it is written for
/// recursive scenarios, e.g. where the callback is called or accessed inside a
/// function that was called because the callback was called.
/// So the callback might be accessed from inside such a function.
/// If you don't need this functionality, just see nytl::Callback.
/// The public interface of this class is identical with nytl::Callback
/// so they can be used interchangeably.
/// The class is not thread-safe in any way.
/// All exceptions from calls are just propagated.
/// The class can not be copied or moved.
///
/// \tparam Signature The signature of the registered functions.
/// Uses the same syntax and semantics as std::function.
/// \tparam ID A connectionID class, see nytl/connection.hpp for examples.
/// See docs/callback.md for specification.
template<typename Signature, typename ID = ConnectionID>
class RecursiveCallback;

/// Callback class typedef using TrackedConnectionID. Enables connections
/// to see when their associated function is unregistered by another
/// connection or because the callback was destroyed.
template<typename Signature> using TrackedRecursiveCallback =
	RecursiveCallback<Signature, TrackedConnectionID>;

// Callback specialization to enable the Ret(Args...) Signature format.
template<typename Ret, typename... Args, typename ID>
class RecursiveCallback<Ret(Args...), ID>
	: public ConnectableT<ID>, public NonCopyable {
public:
	using Signature = Ret(Args...);
	using Connection = ConnectionT<ConnectableT<ID>, ID>;

	RecursiveCallback() = default;
	~RecursiveCallback();

	/// \brief Registers a new Callback function.
	/// \returns A connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::invalid_argument If an empty function target is registered.
	Connection add(std::function<Ret(Args...)>);

	/// \brief Registers a new Callback function with additional connection parameter.
	/// \returns A connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::invalid_argument If an empty function target is registered.
	Connection add(std::function<Ret(Connection, Args...)>);

	/// Calls all registered functions and returns a vector with the returned objects,
	/// or void when this is a void callback.
	/// Will call all the functions registered at the moment of calling, i.e. additional
	/// functions added from within are not called.
	/// Propagates all upcoming exceptions untouched.
	/// Note that this might also propagate exceptions from ID::remove() that are defered
	/// from disconnect.
	auto call(Args...);

	/// Clears all registered functions.
	void clear() noexcept;

	/// Removes the callback function registered with the given id.
	/// Returns whether the function could be found. If the id is invalid or the
	/// associated function was already removed, returns false.
	/// ID::remove() might be defered until all pending iterations (calls/removes)
	/// are done.
	/// Passing an invalid id or an id that was not returned from this
	/// callback is undefined behvaiour.
	bool disconnect(const ID&) noexcept override;

	/// Operator version of add
	template<typename F>
	Connection operator+=(F&& func) {
		return add(std::forward<F>(func));
	}

	/// Operator version of add that previously calls clear.
	template<typename F>
	Connection operator=(F&& func) {
		clear();
		return add(std::forward<F>(func));
	}

	/// Operator version of call.
	auto operator() (Args... a) {
		return call(std::forward<Args>(a)...);
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
	Subscription& emplace() {
		// output at least a warning when subID_ has to be wrapped
		// Usually this should not happen. Bad things can happend then.
		if(subID_ == std::numeric_limits<std::int64_t>::max()) {
			std::cerr << "nytl::RecursiveCallback::emplace: <warning> wrapping subID_\n";
			subID_ = 0;
		}

		// this expression might throw. In this case we have not changed
		// our own state in any bad way
		ID id = {subID_ + 1};

		// emplace at the last position
		// might also throw
		if(subs_.empty()) {
			subs_.emplace_front();
			last_ = subs_.begin();
		} else {
			last_ = subs_.emplace_after(last_);
		}

		++subID_;
		last_->id = {id};
		return *last_;
	}

	// Removes all old functions that could previously
	// not be removed because of an active iteration.
	void removeOld() noexcept {
		++iterationCount_;

		// just remove subscriptions with id <= 0 and update last_
		auto it = subs_.begin();
		while(it != subs_.end() && it->id.get() <= 0) {
			subs_.pop_front();
			it = subs_.begin();
		}

		if(it != subs_.end()) {
			for(auto next = std::next(it); next != subs_.end(); next = std::next(it)) {
				if(next->id.get() <= 0) {
					next->id.removed();
					subs_.erase_after(it);
				} else {
					++it;
				}
			}
		}

		last_ = it;
		--iterationCount_;
	}

	// TODO: maybe cache current size for performance?

	// Upper approximation of the current size.
	// May be larger than the actual size.
	std::size_t size() const {
		return std::distance(subs_.begin(), subs_.end());
	}

	std::forward_list<Subscription> subs_ {}; // all registered subscriptions
	typename decltype(subs_)::iterator last_ {}; // the last entry in subs
	unsigned int iterationCount_ {}; // the number of active iterations (in call)
	std::int64_t subID_ {}; // the highest subscription id given
	std::int64_t callID_ {}; // the highest call id given (see the call function)
};

// - implementation -
template<typename Ret, typename... Args, typename ID>
RecursiveCallback<Ret(Args...), ID>::~RecursiveCallback()
{
	// Output warnings in bad cases.
	// The following can only happen if e.g. deleted from within a
	// call
	if(iterationCount_) {
		std::cerr << "nytl::~RecursiveCallback: iterationCount_: " << iterationCount_ << "\n";
	}

	for(auto& sub : subs_) {
		sub.id.removed();
	}
}

template<typename Ret, typename... Args, typename ID>
ConnectionT<ConnectableT<ID>, ID> RecursiveCallback<Ret(Args...), ID>::
add(std::function<Ret(Args...)> func) {
	if(!func) {
		throw std::invalid_argument("nytl::Callback::add: empty function");
	}

	auto& sub = emplace();
	sub.func = std::move(func);
	return {*this, sub.id};
}

template<typename Ret, typename... Args, typename ID>
ConnectionT<ConnectableT<ID>, ID> RecursiveCallback<Ret(Args...), ID>::
add(std::function<Ret(Connection, Args...)> func)
{
	if(!func) {
		throw std::invalid_argument("nytl::Callback::add: empty function");
	}

	auto& sub = emplace();
	Connection conn {*this, sub.id};
	sub.func = [conn, f = std::move(func)](Args... args) {
		return f(conn, std::forward<Args>(args)...);
	};

	return conn;
}

template<typename Ret, typename... Args, typename ID>
auto RecursiveCallback<Ret(Args...), ID>::call(Args... a)
{
	// wrap callID_ if needed. This is usually not critical (except when
	// there are 2^32 nested calls...)
	callID_ = (callID_ == std::numeric_limits<std::int64_t>::max()) ? 1 : callID_ + 1;
	std::int64_t callid = callID_; // the actual calling id (to include newly removed)
	auto last = last_; // store it (to not iterate over newly added subs)

	// make sure the no list iterators are invalidated while iterating
	++iterationCount_;

	// make sure the iteration count and cleanup done if possible
	// even in the case of an exception.
	auto successGuard = ScopeGuard([&]{
		if(--iterationCount_ == 0) {
			removeOld();
		}
	});

	if constexpr(std::is_same<Ret, void>::value) {
		for(auto it = subs_.begin(); it != subs_.end(); ++it) {
			// if the first check if fasel, -it->id.get() represents the
			// callID during which it was removed. If this is >= than
			// the stored callID, it was removed during or after this
			// call and therefore we still call it
			if(it->id.get() > 0 || -it->id.get() >= callid) {
				it->func(std::forward<Args>(a)...);
				// we will not call functions that were registered after
				// this call started (this is why we store last above)
				if(it == last) {
					break;
				}
			}
		}

	} else { // the same with a return vector
		std::vector<Ret> ret;
		ret.reserve(size());

		for(auto it = subs_.begin(); it != subs_.end(); ++it) {
			if(it->id.get() > 0 || -it->id.get() >= callid) {
				ret.push_back(it->func(std::forward<Args>(a)...));
				if(it == last) {
					break;
				}
			}
		}

		return ret;
	}
}

template<typename Ret, typename... Args, typename ID>
void RecursiveCallback<Ret(Args...), ID>::clear() noexcept
{
	bool remove = iterationCount_ == 0;

	// reset the ids or notify the ids of removal
	for(auto& sub : subs_) {
		if(remove) {
			sub.id.removed();
		} else {
			sub.id.set(-callID_);
		}
	}

	// clear/remove only if no one is currently iterating
	// we only reset end_ (not really needed, cleanup for easier debugging)
	if(remove) {
		subs_.clear();
		last_ = subs_.end();
	}
}

// TODO: noexcept?
template<typename Ret, typename... Args, typename ID>
bool RecursiveCallback<Ret(Args...), ID>::disconnect(const ID& id) noexcept
{
	if(subs_.empty())  {
		return false;
	}

	auto remove = (iterationCount_ == 0);

	// check for first one
	if(subs_.begin()->id.get() == id.get()) {
		if(remove) {
			subs_.begin()->id.removed();
			subs_.pop_front();
			if(subs_.empty()) {
				last_ = subs_.end();
			}
		} else {
			subs_.begin()->id.set(-callID_);
		}

		return true;
	}

	// iterate through subs, always check the next elem (fwd linked list)
	// the end condition constructs a copy of it and increases it to check
	for(auto it = subs_.begin(); true; ++it) {
		auto next = it;
		if(++next == subs_.end())
			break;

		if(next->id.get() == id.get()) {
			if(remove) {
				// set this before id.removed might change stuff
				if(++next == subs_.end()) {
					last_ = it;
				}
				next->id.removed();
				subs_.erase_after(it);
			} else {
				next->id.set(-callID_);
			}

			return true;
		}
	}

	return false;
}

} // namespace nytl

#endif // header guard
