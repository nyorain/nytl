/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Defines the callback and connection classes.

#pragma once

#include <nytl/nonCopyable.hpp>
#include <nytl/compFunc.hpp>

#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <atomic>

namespace nytl
{

class connection;
class connectionRef;

template <class Signature> class callback;

namespace detail
{

class callbackBase
{

friend class ::nytl::connection;
friend class ::nytl::connectionRef;

protected:
    virtual void remove(size_t id) = 0;
};

struct connectionData
{
    connectionData(size_t i) : id(i) {}
    std::atomic<size_t> id {0}; //if id == 0, the connection is not connected
};

}

///\ingroup function
///\brief The connection class represents a connection to a nytl::callback slot.
///\details A connection object is returned when a function is registered in a callback object
///and can then be used to unregister the function and furthermore check whether 
///the callback object is still valid and the function is still registered.
class connection
{
protected:
    template<class T> friend class callback;

	detail::callbackBase* callback_ {nullptr};
    std::shared_ptr<detail::connectionData> data_ {nullptr};

    connection(detail::callbackBase& call, std::shared_ptr<detail::connectionData> data) 
		: callback_(&call), data_(data) {}

public:
	connection() = default;
    ~connection() = default;

    connection(const connection&) = default;
    connection& operator=(const connection&) = default;

    connection(connection&&) = default;
    connection& operator=(connection&&) = default;

	///Unregisters the function associated with this connection from the callback object.
    void destroy()
		{ if(callback_ && connected()) callback_->remove(data_->id); callback_ = nullptr; }

	///Returns whether the function is still registered and the callback is still alive.
    bool connected() const 
		{ return (callback_) && (data_) && (data_->id.load() != 0); }
};

///\ingroup function
///\brief Like Connection representing a registered function but can be used inside callbacks.
///\details Sometimes it may be useful to unregister a callback function while it is called
///(e.g. if the callback function should be called only once) and there is no possibility to
///capture a connection object inside the callback (like e.g. with lambdas) then a connectionRef
///parameter can be added to the beggining of the callbacks function signature with which the
///function can be unregistered from inside itself. A new class is needed for this since 
///if connection is used in a fucntion signature, the callback object can not know if this
///connection object is part of the signature or only there to get a connection to itself.
///So there is no need for generally using this class outside a callback function, connection
///should be used instead since it proved the same functionality.
class connectionRef
{
protected:
    template<class T> friend class callback;

	detail::callbackBase* callback_ {nullptr};
    std::shared_ptr<detail::connectionData> data_ {nullptr};

    connectionRef(detail::callbackBase& call, std::shared_ptr<detail::connectionData> data) 
		: callback_(&call), data_(data) {}
public:
    ~connectionRef() = default;

    connectionRef(const connectionRef& other) = default;
    connectionRef& operator=(const connectionRef& other) = default;

    connectionRef(connectionRef&& other) = default;
    connectionRef& operator=(connectionRef&& other) = default;

	///Disconnected the connection, unregisters the associated function.
    void destroy() const
		{ if(callback_ && connected()) callback_->remove(data_->id); }

	///Returns whether the callback function is still registered.
    bool connected() const 
		{ return (callback_) && (data_->id.load() != 0); }
};

///\ingroup function
///RAII connection class that will disconnect automatically on destruction.
class raiiConnection
{
protected:
	connection connection_ {};

public:
	raiiConnection(const connection& conn) : connection_(conn) {}
	~raiiConnection() { connection_.destroy(); }

	raiiConnection(raiiConnection&& other) : connection_(std::move(other.connection_)) {}
	raiiConnection& operator=(raiiConnection&& other) 
		{ release(); connection_ = std::move(other.connection_); return *this; }

	connection& get() { return connection_; }
	const connection& get() const { return connection_; }
	void release(){ connection_ = {}; }

	bool connected() const { return connection_.connected(); }
	void destroy() { connection_.destroy(); }
};


//TODO make callback threadsafe using a lockfree list as container. There are already shared
//pointers anyway so it should not be that expensive.

///\brief Represents a callback for which listener functions can be registered.
///\ingroup function
///
///\details It is used for registering functions that should be called when
///the callback is triggered. This is intented as more lightweight, easier, more dynmaic and 
///macro-free options to the signal-slot mechanism used by many c++ libraries.
///The temaplte parameter Signature indicated the return types registered fucntions should have
///and the possible parameter they can get.
///
///Registering a callback function returns a connection object which can then be used to unregister
///it an to check whether it is still connected which means that the function is still
///registered (connection objects can be copied so it may have been unregistered by a copied 
///connection) and the callback object is still alive. Any object that can be represented
///by a std::function can be registered at a callback object, so it is impossible to 
///unregister a function only by its functions (std::function cannot be compared for equality),
///that is why connections are used to unregister/check the registered functions.
///
///Registered functions that should be called if the callback is activated must have a signature
///compatible (-> see \c compatibleFunction for more information on the compatible functions 
///conecpt) to Ret(const connectionRef&, Arg...).
///The nytl::connectionRef object can optionally be used to unregister the callback function from
///inside itself when it gets triggered.
///At the moment callback is not fully threadsafe, if one thread calls e.g. call() while another
///one calls add() it may cause undefined behaviour.
template <class Ret, class ... Args> 
class callback<Ret(Args...)> : public detail::callbackBase
{
protected:
    struct callbackSlot
    {
        std::shared_ptr<detail::connectionData> data;
        std::function<Ret(const connectionRef&, Args ...)> func;
    };

protected:
    size_t highestID_ {0};
    std::vector<callbackSlot> slots_;

    virtual void remove(size_t id) override
    {
        if(id == 0)
            return;

        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->data->id == id)
            {
                it->data->id.store(0);
                slots_.erase(it);
                return;
            }

        }
    };

public:
	///Destroys the callback object and removes all registered functions.
    ~callback()
    {
        clear();
    }

    ///Registers a function without returning a connection object.
    connection operator+=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        return add(func);
    };

	///Resets all registered function and sets the given one as only callback function.
    connection operator=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        clear();
        return add(func);
    };

	///\brief Registers a callback function.
	///\details The function must have a compatible signature to the callbacks one but
	///may additionally have a connectionRef parameter as first one which can then
	///be used to unregister the function from within itself.
	///\return A connection object for the registered function which can be used to 
	///unregister it and check if it is registered, see \c connection for more information.
    connection add(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<detail::connectionData>(++highestID_);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return connection(*this, ptr);
    };

	///Calls all registered functions and returns a Vector with the returned objects.
    std::vector<Ret> call(Args ... a)
    {
        auto Vec = slots_; //if called functions manipulate callback

        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(auto& slot : Vec)
            ret.push_back(slot.func(connectionRef(*this, slot.data), a ...));

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
        return call(a ...);
    }
};


//The callback specialization for a void return type.
//\details There has to be a specialization since call cannot return a std::vector of void. 
template <class ... Args> 
class callback<void(Args...)> : public detail::callbackBase
{
protected:
    struct callbackSlot
    {
        std::shared_ptr<detail::connectionData> data;
        std::function<void(const connectionRef&, Args ...)> func;
    };

protected:
    size_t highestID {0};
    std::vector<callbackSlot> slots_;

    virtual void remove(size_t id) override
    {
        if(id == 0)
            return;

        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->data->id == id)
            {
                it->data->id.store(0);
                slots_.erase(it);
                return;
            }
        }
    };

public:
    ~callback()
    {
        clear();
    }

    connection operator+=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        return add(func);
    };

    connection operator=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        clear();
        return add(func);
    };

    connection add(compFunc<void(const connectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<detail::connectionData>(++highestID);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return connection(*this, ptr);
    };

    void call(Args ... a)
    {
        auto Vec = slots_; 

        for(auto& slot : Vec)
            slot.func(connectionRef(*this, slot.data), a ...);
    };

    void clear()
    {
        slots_.clear();
    }

    void operator() (Args... a)
    {
        call(a ...);
    }
};

}

