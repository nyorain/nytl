// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines the Callback and Connection classes.

#pragma once

#ifndef NYTL_INCLUDE_CALLBACK_HPP
#define NYTL_INCLUDE_CALLBACK_HPP

#include <nytl/compFunc.hpp>
#include <nytl/connection.hpp>

#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <atomic>

namespace nytl
{

//First declaration - undefined.
//Signature must have the format ReturnType(Args...)
template <class Signature> class Callback;

///\ingroup function
///Like Connection representing some kind of connection but does not own (i.e. destroy) the
///hold connection and is specially used as first possible nytl::Callback function
///parameter.
///Sometimes it may be useful to unregister a Callback function while it is called
///(e.g. if the Callback function should be called only once) and there is no possibility to
///capture a Connection object inside the Callback (like e.g. with lambdas) then a ConnectionRef
///parameter can be added to the beggining of the Callbacks function signature with which the
///function can be unregistered from inside itself. A new class is needed for this since
///if Connection is used in a function signature, the Callback object can not know if this
///Connection object is part of the signature or only there to get a Connection to itself.
///So there is no need for generally using this class outside a Callback function, Connection
///should be used instead since it proved the same functionality.
template<typename Base, typename ID>
class BasicConnectionRef
{
public:
	BasicConnectionRef() = default;
	BasicConnectionRef(Base& conn, ID id) noexcept : conn_(&conn), id_(id) {}
	~BasicConnectionRef() = default;

	BasicConnectionRef(const BasicConnectionRef& lhs) noexcept = default;
	BasicConnectionRef& operator=(const BasicConnectionRef& lhs) noexcept = default;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	void connected() const { return (conn_); }

	Base& connectable() const { return *conn_; }
	ID id() const { return id_; }

protected:
	Base* conn_ {};
	ID id_ {};
};

using ConnectionRef = BasicConnectionRef<Connectable, ConnectionID>;


///\brief Represents a Callback for which listener functions can be registered.
///\ingroup function
///
///\details It is used for registering functions that should be called when
///the Callback is triggered. This is intented as more lightweight, easier, more dynmaic and
///macro-free options to the signal-slot mechanism used by many c++ libraries.
///The temaplte parameter Signature indicated the return types registered fucntions should have
///and the possible parameter they can get.
///
///Registering a Callback function returns a unique connection id which can either
///totally ignored, dealt with manually or wrapped into a nytl::Connection guard.
///The returned id can be used to unregister the function.
///Any object that can be represented by a std::function can be registered at a Callback object,
///so it is impossible to unregister a function only by its functions
///(std::function cannot be compared for equality), that is why unique ids are used to
///unregister/check the registered functions.
///
///Registered functions that should be called if the Callback is activated must have a signature
///compatible (-> see \c CompatibleFunction for more information on the compatible functions
///conecpt) to Ret(CbConnRef&&, Arg...).
///The nytl::CbConnRef object can optionally be used to unregister the Callback function from
///inside itself when it gets triggered. This (as well as adding a new callback
///function from inside a callback function) can be done safely.
///The class is not designed threadsafe, if one thread calls e.g. call() while another
///one calls add() it may cause undefined behaviour.
template <class Ret, class ... Args>
class Callback<Ret(Args...)> : public Connectable
{
public:
	using FuncArg = CompFunc<Ret(ConnectionRef&&, Args...)>;
	using ID = ConnectionID;

public:
	///Destroys the Callback object and removes all registered functions.
	virtual ~Callback(){ clear(); }

	///Registers a function without returning a Connection object.
	///\sa add
	Connection operator+=(FuncArg func)
	{
		return add(func);
	};

	///Resets all registered function and sets the given one as only Callback function.
	///\sa add
	Connection operator=(FuncArg func)
	{
		clear();
		return add(func);
	};

	///\brief Registers a Callback function.
	///\details The function must have a compatible signature to the Callbacks one but
	///may additionally have a ConnectionRef parameter as first one which can then
	///be used to unregister the function from within itself.
	///\return A unique connection id for the registered function which can be used to
	///unregister it and check if it is registered.
	///\sa Connection
	Connection add(FuncArg func)
	{
		slots_.emplace_back();
		auto id = ++reinterpret_cast<std::uintptr_t&>(highestID_);

		slots_.back().id = reinterpret_cast<ID>(id);
		slots_.back().func = func.function();

		return {*this, slots_.back().id};
	};

	///Calls all registered functions and returns a Vector with the returned objects.
	std::vector<Ret> call(Args... a)
	{
		auto cpy = slots_; //if called functions manipulate Callback

		std::vector<Ret> ret;
		ret.reserve(cpy.size());

		for(auto& slot : cpy)
			ret.push_back(slot.func({*this, slot.id}, std::forward<Args>(a)...));

		return ret;
	};

	///Clears all registered functions.
	void clear()
	{
		slots_.clear();
	}

	///Operator version of call. Calls all registered functions and return their returned objects.
	std::vector<Ret> operator() (Args... a)
	{
		return call(std::forward<Args>(a)...);
	}

	///Removes the callback function registered with the given id.
	///Returns whether the function could be found. If the id is invalid or the
	///associated function was already removed, returns false.
	bool disconnect(ID id) override
	{
		if(id == nullptr) return false;
		for(auto it = slots_.begin(); it != slots_.end(); ++it)
		{
			if(it->id == id)
			{
				it->id = nullptr;
				slots_.erase(it);
				return true;
			}
		}

		return false;
	};

protected:
	struct CallbackSlot
	{
		ConnectionID id;
		std::function<Ret(ConnectionRef&&, Args...)> func;
	};

	ConnectionID highestID_ {};
	std::vector<CallbackSlot> slots_;
};


//The Callback specialization for a void return type.
//\details There has to be a specialization since call cannot return a std::vector of void.
template <typename... Args>
class Callback<void(Args...)> : public Connectable
{
public:
	using FuncArg = CompFunc<void(ConnectionRef&&, Args...)>;
	using ID = ConnectionID;

public:
	virtual ~Callback() { clear(); }

	Connection operator+=(FuncArg func)
	{
		return add(func);
	};

	Connection operator=(FuncArg func)
	{
		clear();
		return add(func);
	};

	Connection add(FuncArg func)
	{
		slots_.emplace_back();
		auto id = ++reinterpret_cast<std::uintptr_t&>(highestID_);

		slots_.back().id = reinterpret_cast<ID>(id);
		slots_.back().func = func.function();

		return {*this, slots_.back().id};
	};

	void call(Args... a)
	{
		auto cpy = slots_;
		for(auto& slot : cpy) slot.func({*this, slot.id}, std::forward<Args>(a)...);
	};

	void clear()
	{
		slots_.clear();
	}

	void operator() (Args... a)
	{
		call(std::forward<Args>(a)...);
	}

	bool disconnect(ID id) override
	{
		if(id == nullptr) return false;
		for(auto it = slots_.begin(); it != slots_.end(); ++it)
		{
			if(it->id == id)
			{
				it->id = nullptr;
				slots_.erase(it);
				return false;
			}
		}

		return true;
	};

protected:
	struct CallbackSlot
	{
		ConnectionID id;
		std::function<void(ConnectionRef&&, Args...)> func;
	};

	ConnectionID highestID_ {0};
	std::vector<CallbackSlot> slots_;
};


//Makes implicit conversion of a nytl::ConnectionRef to e.g. a std::any object that
//is used as first signature parameter impossible.
template<typename Base, typename ID, typename B>
struct ConvertException<nytl::BasicConnectionRef<Base, ID>, B> : public std::false_type {};

template<typename Base, typename ID>
struct ConvertException<nytl::BasicConnectionRef<Base, ID>, nytl::BasicConnectionRef<Base, ID>>
	: public std::true_type {};

}

#endif //header guard
