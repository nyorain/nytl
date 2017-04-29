// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Defines the Callback template class and void specialization.

#pragma once

#ifndef NYTL_INCLUDE_RECURSIVE_CALLBACK
#define NYTL_INCLUDE_RECURSIVE_CALLBACK

#include <nytl/connection.hpp> // nytl::BasicConnection
#include <nytl/nonCopyable.hpp> // nytl::NonCopyable

#include <functional> // std::function
#include <forward_list> // std::vector
#include <utility> // std::move
#include <cstdint> // std::uintptr_t
#include <type_traits> // std::is_same
#include <vector> // std::vector

namespace nytl {

template<typename Signature, typename ID = ConnectionID> class Callback;

// TODO: use std::pmr for more efficient memory allocations (?)
//
// Concept for ID:
//  - default constructor
//  - constructor only with std::size_t (or sth compatible) for the id
//  - copyable/copy-assignable
//  - reset(): Will be called to reset the connection id. Might be called multiple times.
//  - valid() const: Should return whether the connection id is valid.

// TODO: class/param concept doc!
/// Callback List.
template<typename Ret, typename... Args, typename ID>
class Callback<Ret(Args...), ID> : public ConnectableT<ID> {
public:
	using Signature = Ret(Args...);
	using Connection = ConnectionT<ConnectableT<ID>, ID>;

	Callback() = default;

	Callback(const Callback& other) = delete;
	Callback& operator=(const Callback& other) = delete;

	Callback(Callback&& other) = delete;
	Callback& operator=(Callback&& other) = delete;

	~Callback()
	{
		// Output warnings in bad cases.
		if(iterationCount_)
			std::cerr << "nytl::~Callback: iterationCount_ != 0\n";

		// We clear manually instead of calling clear so we can at least
		// try to reset every id in the case of exceptions.
		for(auto& sub : subs_) {
			try {
				sub.id.reset();
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
		auto last = end_;
		++iterationCount_;

		if constexpr(std::is_same<Ret, void>::value) {
			for(auto it = subs_.begin(); it != subs_.end(); ++it) {
				if(!it->id.valid()) continue;
				it->func(std::forward<Args>(a)...);
				if(it == last) break;
			}

			if(--iterationCount_ == 0)
				removeOld();
		} else {
			std::vector<Ret> ret;
			ret.reserve(size());

			for(auto it = subs_.begin(); it != subs_.end(); ++it) {
				if(!it->id.valid()) continue;
				ret.push_back(it->func(std::forward<Args>(a)...));
				if(it == last) break;
			}

			if(--iterationCount_ == 0)
				removeOld();

			return ret;
		}
	}

	/// Clears all registered functions.
	/// Will propagate exceptions from ID::reset().
	void clear()
	{
		// first reset the ids and functions
		for(auto& sub : subs_) {
			sub.id.reset();
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
			if(iterationCount_) subs_.begin()->id.reset();
			else subs_.pop_front();

			return true;
		}

		// iterate through subs, always check the next elem (fwd linked list)
		// the end condition constructs a copy of it and increases it to check
		for(auto it = subs_.begin(); true; ++it) {
			auto next = it;
			if(++next == subs_.end())
				break;

			if(next->id == id) {
				if(iterationCount_) next->id.reset();
				else subs_.erase_after(it);

				return true;
			}
		}

		// TODO: remove debug output
		std::cerr << "nytl::Callback::disonnect: could not find id\n";
		return false;
	}

protected:
	/// Represents one callback subscription entry.
	/// Invalid (formally removed) when id is not valid.
	/// Note that these means that alternative ID classes can
	/// remove subscriptions from the outside without actively
	/// calling disconnect.
	/// We cannot touch func while any iteration is active.
	struct Subscription {
		std::function<Ret(Args...)> func;
		ID id;
	};

	/// Emplaces a new subscription for the given function.
	Subscription& emplace()
	{
		if(subs_.empty()) {
			subs_.emplace_front();
			end_ = subs_.begin();
		} else {
			end_ = subs_.emplace_after(end_);
		}

		end_->id = {++highestID_};
		return *end_;
	}

	/// Removes all old (i.e. !sub.func) functions that could previously
	/// not be removed.
	void removeOld()
	{
		subs_.remove_if([](const auto& sub){ return !sub.id.valid(); });
	}

	/// Upper approximation of the current size.
	/// May be larger than the actual size.
	std::size_t size() const
	{
		std::size_t ret = 0u;
		for(auto it = subs_.begin(); it != subs_.end(); ++it)
			++ret;
		return ret;
	}

	std::forward_list<Subscription> subs_ {};
	typename decltype(subs_)::iterator end_ {}; // the last entry in subs
	unsigned int iterationCount_ {}; // the number of active iterations (in call)
	std::size_t highestID_ {};
};

} // namespace nytl

#endif
