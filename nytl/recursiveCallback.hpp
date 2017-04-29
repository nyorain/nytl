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
#include <vector> // std::vector
#include <utility> // std::move
#include <cstdint> // std::uintptr_t
#include <type_traits> // std::is_same

namespace nytl {

template<typename Signature, typename ID = ConnectionID> class Callback;

// TODO: use std::pmr for more efficient memory allocations.
// Every call to add or disconnect will call new/delete 2 times!
template<typename Ret, typename... Args, typename CID>
class Callback<Ret(Args...), CID> : public BasicConnectable<CID> {
public:
	using ID = CID;
	using Signature = Ret(Args...);
	using Conn = BasicConnection<BasicConnectable<ID>, ID>;

	Callback() = default;

	Callback(const Callback& other) = delete;
	Callback& operator=(const Callback& other) = delete;

	Callback(Callback&& other) = delete;
	Callback& operator=(Callback&& other) = delete;

	~Callback()
	{
		clear();
	}

	/// \brief Registers a new function in the same way add does.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	template<typename F>
	Conn operator+=(F&& func)
	{
		return add(std::forward<F>(func));
	}

	/// \brief Resets all registered function and sets the given one as only Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	template<typename F>
	Conn operator=(F&& func)
	{
		clear();
		return add(std::forward<F>(func));
	}

	/// \brief Registers a new Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	Conn add(std::function<Ret(Args...)> func)
	{
		auto& slot = emplace();
		slot.func = std::move(func);
		return {*this, slot.id};
	}

	/// \brief Registers a new Callback function with additional connection parameter.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	Conn add(std::function<Ret(Conn, Args...)> func)
	{

		auto& slot = emplace();
		Conn conn {*this, slot.id};

		slot.func = [conn, id = slot.id, f = std::move(func)](Args... args) {
			return f(conn, std::forward<Args>(args)...);
		};

		return conn;
	}

	/// Calls all registered functions and returns a Vector with the returned objects.
	auto call(Args... a)
	{
		CallIter iter;
		iter.above = callIter_;
		iter.last = last_;

		callIter_ = &iter;
		auto it = slots_;

		if constexpr(std::is_same<Ret, void>::value) {
			while(it) {
				iter.current = it;
				iter.next = it->next;

				// it->func({*this, it->id}, std::forward<Args>(a)...);
				it->func(std::forward<Args>(a)...);
				if(iter.owned) {
					delete it;
					iter.owned = false;
				}

				if(it == iter.last)
					break;

				it = iter.next;
			}

			callIter_ = iter.above;
			return;
		} else {
			std::vector<Ret> ret;
			ret.reserve(size_);

			while(it) {
				iter.current = it;
				iter.next = it->next;

				// ret.push_back(it->func({*this, it->id}, std::forward<Args>(a)...));
				ret.push_back(it->func(std::forward<Args>(a)...));
				if(iter.owned) {
					delete it;
					iter.owned = false;
				}

				if(it == iter.last)
					break;

				it = iter.next;
			}

			callIter_ = iter.above;
			return ret;
		}
	}

	/// Clears all registered functions.
	void clear()
	{
		auto it = slots_;
		while(it) {
			auto tmp = it;
			bool nodelete = false;
			if(callIter_) nodelete = callIter_->checkErase(it, nullptr);

			it->id.reset();
			it = it->next;

			if(!nodelete)
				delete tmp;
		}

		slots_ = nullptr;
		last_ = nullptr;
		size_ = 0;
	}

	/// Operator version of call. Calls all registered functions and return their returned objects.
	auto operator() (Args... a)
	{
		return call(std::forward<Args>(a)...);
	}

	/// Returns the current number of registered functions.
	std::size_t size() const
	{
		return size_;
	}

	/// Removes the callback function registered with the given id.
	/// Returns whether the function could be found. If the id is invalid or the
	/// associated function was already removed, returns false.
	bool disconnect(const ID& id) override
	{
		if(id == ID {} || !slots_) return false;

		// first
		if(slots_->id == id) {
			bool nodelete = false;
			if(callIter_) nodelete = callIter_->checkErase(slots_, nullptr);

			slots_->id.reset();
			auto tmp = slots_;
			slots_ = slots_->next;

			if(!nodelete)
				delete tmp;

			--size_;
			if(!slots_)
				last_ = nullptr;

			return true;
		}

		// iterate
		for(auto it = slots_; it->next; it = it->next) {
			if(it->next->id == id) {
				bool nodelete = false;
				if(callIter_) nodelete = callIter_->checkErase(it->next, it);

				it->next->id.reset();
				auto tmp = it->next;
				it->next = tmp->next;

				if(!nodelete)
					delete tmp;

				--size_;
				if(!it->next)
					last_ = nullptr;

				return true;
			}
		}

		return false;
	}

protected:
	// Represents one registered callback function with id
	// Structured as a linked list since this is the only structure
	// allowing efficient recursive using/adding/removing.
	struct CallbackSlot {
 		ID id;
		std::function<Ret(Args...)> func;
		CallbackSlot* next;
	};

	CallbackSlot& emplace()
	{
		auto id = ++highestID_;
		++size_;

		if(!slots_) {
			slots_ = new CallbackSlot();
			slots_->id = {id};
			last_ = slots_;
			return *slots_;
		}

		auto slot = slots_;
		while(slot->next) slot = slot->next;
		slot->next = new CallbackSlot();
		slot->next->id = {id};
		last_ = slot->next;
		return *slot->next;
	}


	// Represents one call function on the stack.
	// Implemented this way to allow nesting calls to the same object as well
	// as being able to add/disconnect functions from within a call function.
	// Done without any memory allocation by just using the stack.
	struct CallIter {
		CallbackSlot* current {}; // the current slot
		CallbackSlot* next {}; // the next slot
		CallbackSlot* last {}; // the last slot to do

		CallIter* above {}; // optional pointer to the CallIter from the above iteration
		bool owned {};

		// Will be called from diconnect to signal that the given slot has been removed.
		// has been disconnected. This will update the current value if needed and signal
		// the CallIter above (if there is one).
		bool checkErase(CallbackSlot* slot, CallbackSlot* before, bool used = false)
		{
			if(slot == next) next = slot->next;
			if(slot == last) last = before;
			if(slot == current && !used) used = owned = true;
			if(above) return above->checkErase(slot, before, used);
			return used;
		}
	};

	std::size_t highestID_ {};
	std::size_t size_ {};
	CallbackSlot* slots_ {};
	CallbackSlot* last_ {};
	CallIter* callIter_ {}; // pointer to iter inside the lowest (last called) this->call
};

} // namespace nytl

#endif
