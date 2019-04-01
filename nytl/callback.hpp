// Copyright (c) 2017-2019 nyorain
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
#include <utility> // std::move
#include <cstdint> // std::uint64_t
#include <cstddef> // std::size_t
#include <type_traits> // std::is_same
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include <iostream> // std::cerr
#include <stdexcept> // std::logic_error

namespace nytl {

/// A Callback is a collection of functions with the given signature.
/// Everyone can add functions or remove his registered function using
/// the id returned from registering it.
/// All callbacks can then be called with the required arguments
/// This callback class does not allow ANY recursive operations on it,
/// so e.g. removing a registered function or register a new function
/// from within a callback call will lead to
/// undefined behvaiour. If you need this recursive functionality (in any way,
/// even if from the connectionID class), see nytl/recursiveCallback.
/// The public interface of this class is mostly identical with RecursiveCallback
/// so they can be used interchangeably (exceptions documented).
/// The class is not thread-safe in any way.
/// All exceptions from calls are just propagated.
/// The class can not be copied or moved.
///
/// \tparam Signature The signature of the registered functions.
/// Uses the same syntax and semantics as std::function.
/// \tparam ID A connectionID class, see nytl/connection.hpp for examples.
/// See docs/callback.md for specification.
template<typename Signature, typename ID = ConnectionID>
class Callback;

/// Callback class typedef using TrackedConnectionID. Enables connections
/// to see when their associated function is unregistered by another
/// connection or because the callback was destroyed.
template<typename Signature> using TrackedCallback =
	Callback<Signature, TrackedConnectionID>;

// Callback specialization to enable the Ret(Args...) Signature format.
template<typename Ret, typename... Args, typename ID>
class Callback<Ret(Args...), ID>
	: public ConnectableT<ID>, public NonCopyable {
public:
	/// ! Definition not present in RecursiveCallback
	/// Represents one callback subscription entry.
	/// Invalid (formally removed) when id is not valid.
	/// Note that these means that alternative ID classes can
	/// remove subscriptions from the outside without actively
	/// calling disconnect.
	struct Subscription {
		std::function<Ret(Args...)> func;
		ID id;
	};

	using Signature = Ret(Args...);
	using Connection = ConnectionT<ConnectableT<ID>, ID>;

public:
	Callback() = default;
	~Callback();

	/// \brief Registers a new Callback function.
	/// \returns A connection id for the registered function which can be used to
	/// unregister it.
	/// \throws std::invalid_argument If an empty function target is registered.
	Connection add(std::function<Ret(Args...)>);

	/// Calls all registered functions and returns a vector with the returned objects,
	/// or void when this is a void callback.
	/// Will call all the functions registered at the moment of calling, i.e. additional
	/// functions added from within are not called.
	/// If a registered function throws, the exception is not caught, i.e. the following
	/// handlers will not be called.
	auto call(Args...);

	/// Clears all registered functions.
	void clear() noexcept;

	/// Removes the callback function registered with the given id.
	/// Returns whether the function could be found. If the id is invalid or the
	/// associated function was already removed, returns false.
	/// Prefer to use this indirectly using some Connection object.
	/// Propagates exceptions from ID::removed() but always removes the associated
	/// function. Passing an invalid id or an id that was not returned from this
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

	/// ! Not present in RecursiveCallback
	/// Returns the internal vector of registered subscriptions
	/// Can be used to e.g. call it more efficiently (without creating a vector) or
	/// with custom exception handling.
	const auto& subscriptions() const {
		return subs_;
	}

protected:
	std::vector<Subscription> subs_ {}; // all subscriptions, ordered by id
	std::int64_t subID_ {}; // the highest subscription id given
};

// - implementation
template<typename Ret, typename... Args, typename ID>
Callback<Ret(Args...), ID>::~Callback()
{
	for(auto& sub : subs_) {
		sub.id.removed();
	}
}

template<typename Ret, typename... Args, typename ID>
ConnectionT<ConnectableT<ID>, ID> Callback<Ret(Args...), ID>::
add(std::function<Ret(Args...)> func) {
	if(!func) {
		throw std::invalid_argument("nytl::Callback::add: empty function");
	}

	// output at least a warning when subID_ has to be wrapped
	// Usually this should not happen. Bad things can happen then.
	if(subID_ == std::numeric_limits<std::int64_t>::max()) {
		std::cerr << "nytl::Callback::emplace: <warning> wrapping subID_\n";
		subID_ = 0;
	}

	// this expression might throw. In this case we have not changed
	// our own state in any bad way
	ID id = {subID_ + 1};

	++subID_;
	subs_.emplace_back();
	subs_.back().id = id;
	subs_.back().func = std::move(func);
	return {*this, id};
}

template<typename Ret, typename... Args, typename ID>
auto Callback<Ret(Args...), ID>::call(Args... a)
{
	// the first continue check is needed to not call functions that were
	// removed before this call started but call functions that were removed
	// only by some other callback function
	if constexpr(std::is_same<Ret, void>::value) {
		for(auto& func : subs_)
			func.func(std::forward<Args>(a)...);
	} else {
		std::vector<Ret> ret;
		ret.reserve(subs_.size());

		for(auto& func : subs_) {
			ret.push_back(func.func(std::forward<Args>(a)...));
		}

		return ret;
	}
}

template<typename Ret, typename... Args, typename ID>
void Callback<Ret(Args...), ID>::clear() noexcept
{
	// notify the ids of removal
	for(auto& sub : subs_) {
		sub.id.removed();
	}
	subs_.clear();
}

template<typename Ret, typename... Args, typename ID>
bool Callback<Ret(Args...), ID>::disconnect(const ID& id) noexcept
{
	constexpr auto pred = [](const auto& s1, const auto& s2) {
		return s1.id.get() < s2.id.get();
	};

	// we know that id's are ordered
	auto ds = Subscription{{}, id}; // dummy
	auto range = std::equal_range(subs_.begin(), subs_.end(), ds, pred);
	if(range.first == range.second) {
		return false;
	}

	// we can assume that there is only one item in the range
	range.first->id.removed();
	subs_.erase(range.first);
	return true;
}

} // namespace nytl

#endif // header guard
