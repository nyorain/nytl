// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Defines the Callback template class and void specialization.

#pragma once

#ifndef NYTL_INCLUDE_CALLBACK
#define NYTL_INCLUDE_CALLBACK

#include <nytl/connection.hpp> // nytl::BasicConnection

#include <functional> // std::function
#include <vector> // std::vector
#include <utility> // std::move

namespace nytl {

// First declaration - undefined.
// Signature must have the format ReturnType(Args...)
template<class Signature> class Callback;

/// \brief Represents a Callback for which listener functions can be registered.
///
/// Used for registering functions that should be called when the Callback is triggered.
/// This is intented as more lightweight, easier, more dynmaic and
/// macro-free options to the signal-slot mechanism used by many c++ libraries.
/// The template parameter Signature indicated the return types registered functions should have
/// and the parameters they take.
///
/// Registering a Callback function returns a unique connection id which can either
/// totally ignored, dealt with manually or wrapped into a nytl::Connection guard.
/// The returned id can be only be used to unregister the function.
/// Any object that can be represented by a std::function can be registered at a Callback object,
/// so it is impossible to unregister a function only by knowing its function
/// (std::function cannot be compared for equality), that is why unique ids are used to
/// unregister/check the registered functions.
///
/// Registered functions that should be called if the Callback is activated must have the
/// signature Ret(Args...). Alternatively, the function can have the signature
/// Ret(nytl::Connection, Args...) in which case it will receive an additional connection parameter
/// that can then be used to disconnect the callback connection for the called function from
/// withtin itself.
/// This (as well as adding a new callback function from inside a callback function of the
/// same callback object) can be done safely. Calls to the callback can also safely be nested,
/// so e.g. a registered callback function can trigger the callback again from within (but should
/// not do so too often or it will result in a stack overflow).
///
/// The class is not designed threadsafe, if one thread calls e.g. call() while another
/// one calls add() it may cause undefined behaviour.
template<class Ret, class ... Args>
class Callback<Ret(Args...)> : public Connectable {
public:
	/// \brief Registers a new function in the same way add does.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	template<typename F>
	Connection operator+=(F&& func)
	{
		return add(std::forward<F>(func));
	}

	/// \brief Resets all registered function and sets the given one as only Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	template<typename F>
	Connection operator=(F&& func)
	{
		clear();
		return add(std::forward<F>(func));
	}

	/// \brief Registers a new Callback function.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	Connection add(std::function<Ret(Args...)> func)
	{
		emplace();
		slots_.back().func = [f = std::move(func)](Connection, Args... args) {
			return f(std::forward<Args>(args)...);
		};

		return {*this, slots_.back().id};
	}

	/// \brief Registers a new Callback function with additional connection parameter.
	/// \returns A unique connection id for the registered function which can be used to
	/// unregister it.
	Connection add(std::function<Ret(Connection, Args...)> func)
	{
		emplace();
		slots_.back().func = std::move(func);
		return {*this, slots_.back().id};
	}

	/// Calls all registered functions and returns a Vector with the returned objects.
	std::vector<Ret> call(Args... a)
	{
		// we need the implementation with a CallIter instead of e.g. a range-based
		// for loop, since slots can be removed are added from the called functions.
		// See disconnect function.
		// The whole mechanism works by pushing a CallIter on the stack from this function,
		// storing a pointer to it inside the class and using this point from
		// disconnect to potentially modify the current iteration index.
		// If this function was called in a nested way all existent CallIters will be
		// signaled (as in a linked list, except only on the stack).
		// Provide almost no overhead when nested calls or removes are not used
		// The caller only pays for what he uses and simply triggering the callback
		// is not that expensive.

		CallIter iter;
		iter.above = callIter_;
		callIter_ = &iter;
		auto& idx = iter.current;

		std::vector<Ret> ret;
		ret.reserve(slots_.size());

		while(idx < slots_.size()) {
			auto& slot = slots_[idx];
			++idx;
			ret.push_back(slot.func({*this, slot.id}, std::forward<Args>(a)...));
		}

		callIter_ = iter.above;
	}

	/// Clears all registered functions.
	void clear()
	{
		slots_.clear();
	}

	/// Operator version of call. Calls all registered functions and return their returned objects.
	std::vector<Ret> operator() (Args... a)
	{
		return call(std::forward<Args>(a)...);
	}

	/// Removes the callback function registered with the given id.
	/// Returns whether the function could be found. If the id is invalid or the
	/// associated function was already removed, returns false.
	bool disconnect(ConnectionID id) override
	{
		if(id == nullptr) return false;
		for(auto it = slots_.begin(); it != slots_.end(); ++it) {
			if(it->id == id) {
				it->id = nullptr;
				slots_.erase(it);
				if(callIter_) callIter_->checkErase(it - slots_.begin());
				return true;
			}
		}

		return false;
	};

protected:
	void emplace()
	{
		slots_.emplace_back();
		auto id = ++reinterpret_cast<std::uintptr_t&>(highestID_);
		slots_.back().id = reinterpret_cast<ConnectionID>(id);
	}

	// Represents one registered callback function with id
	struct CallbackSlot {
		ConnectionID id;
		std::function<Ret(Connection, Args...)> func;
	};

	// Represents one call function on the stack.
	// Implemented this way to allow nesting calls to the same object as well
	// as being able to add/disconnect functions from within a call function.
	// Done without any memory allocation by just using the stack.
	struct CallIter {
		unsigned int current {}; // current iteration index
		CallIter* above {}; // optional pointer to the CallIter from the above iteration

		// Will be called from diconnect to signal that the entry with the given id
		// has been disconnected. This will update the current value if needed and signal
		// the CallIter above (if there is one).
		void checkErase(unsigned int id)
		{
			if(id < current) --current;
			if(above) above->checkErase(id);
		}
	};

	ConnectionID highestID_ {};
	std::vector<CallbackSlot> slots_;
	CallIter* callIter_ {}; // pointer to iter inside the lowest (last called) this->call
};


// Callback specialization for a void return type.
template<typename... Args>
class Callback<void(Args...)> : public Connectable {
public:
	template<typename F>
	Connection operator+=(F&& func)
	{
		return add(std::forward<F>(func));
	}

	template<typename F>
	Connection operator=(F&& func)
	{
		clear();
		return add(std::forward<F>(func));
	}

	Connection add(std::function<void(Args...)> func)
	{
		emplace();
		slots_.back().func = [f = std::move(func)](Connection, Args... args) {
			return f(std::forward<Args>(args)...);
		};

		return {*this, slots_.back().id};
	}

	Connection add(std::function<void(Connection, Args...)> func)
	{
		emplace();
		slots_.back().func = std::move(func);
		return {*this, slots_.back().id};
	}

	void call(Args... a)
	{
		CallIter iter;
		iter.above = callIter_;
		callIter_ = &iter;
		auto& idx = iter.current;

		while(idx < slots_.size()) {
			auto& slot = slots_[idx];
			++idx;
			slot.func({*this, slot.id}, std::forward<Args>(a)...);
		}

		callIter_ = iter.above;
	}

	void clear()
	{
		slots_.clear();
	}

	void operator() (Args... a)
	{
		call(std::forward<Args>(a)...);
	}

	bool disconnect(ConnectionID id) override
	{
		if(id == nullptr) return false;
		for(auto it = slots_.begin(); it != slots_.end(); ++it) {
			if(it->id == id) {
				it->id = nullptr;
				slots_.erase(it);
				if(callIter_) callIter_->checkErase(it - slots_.begin());
				return true;
			}
		}

		return false;
	};

protected:
	void emplace()
	{
		slots_.emplace_back();
		auto id = ++reinterpret_cast<std::uintptr_t&>(highestID_);
		slots_.back().id = reinterpret_cast<ConnectionID>(id);
	}

	struct CallbackSlot {
		ConnectionID id;
		std::function<void(Connection, Args...)> func;
	};

	struct CallIter {
		unsigned int current {};
		CallIter* above {};

		void checkErase(unsigned int id)
		{
			if(id < current) --current;
			if(above) above->checkErase(id);
		}
	};

	ConnectionID highestID_ {};
	std::vector<CallbackSlot> slots_;
	CallIter* callIter_ {};
};


} // namespace nytl

#endif //header guard
