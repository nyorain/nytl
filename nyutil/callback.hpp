#pragma once

#include <nyutil/nonCopyable.hpp>
#include <nyutil/compFunc.hpp>

#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <atomic>

namespace nyutil
{

class connection;
class connectionRef;
class slot;
template <class> class callback;
template <class> class tsafeCallback;

//class callback should be thread safe
//atm implemented using expensive mutexes etc., should change to lock free containers (deque/queue) later on
//when cheap implementation found, tsafe callback and callback sohuld be merged into one threadsafe callback class

//callbackBase//////////////////////////////////////
class callbackBase
{

friend class connection;
friend class connectionRef;

protected:
    virtual void remove(size_t id) = 0;
};

//connectionData
struct connectionData
{
    connectionData(size_t i) : id(i) {}
    std::atomic<size_t> id {0}; //if id == 0, the connection is not connected
};

//connection//////////////////////////////////////////
class connection
{
protected:
    template<class T> friend class callback;
    template<class T> friend class tsafeCallback;

    callbackBase* callback_ {nullptr};
    std::shared_ptr<connectionData> data_ {nullptr};

    explicit connection(callbackBase& call, std::shared_ptr<connectionData> data) : callback_(&call), data_(data) {}
public:
    ~connection(){}

    connection(const connection&) = default;
    connection& operator=(const connection&) = default;

    void destroy(){ if(callback_ && connected()) callback_->remove(data_->id); callback_ = nullptr; }
    bool connected() const { return data_->id.load() != 0; }
};

//connectionRef for destroying a connection inside a callback/////////////////////
//exactly the same class, only used for this purpose to be able to use it in compFunc without overriding a connection parameter

class connectionRef
{
protected:
    template<class T> friend class callback;
    template<class T> friend class tsafeCallback;

    callbackBase* callback_ {nullptr};
    std::shared_ptr<connectionData> data_ {nullptr};

    explicit connectionRef(callbackBase& call, std::shared_ptr<connectionData> data) : callback_(&call), data_(data) {}
public:
    ~connectionRef() = default;

    connectionRef(const connectionRef& other) = default;
    connectionRef& operator=(const connectionRef& other) = default;

    void destroy(){ if(callback_ && connected()) callback_->remove(data_->id); callback_ = nullptr; }
    bool connected() const { return data_->id.load() != 0; }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//non-threadsafe callback/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class  Ret, class ... Args> class callback<Ret(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        std::shared_ptr<connectionData> data;
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
    ~callback()
    {
        clear();
    }

    //adds an callback by += operator
    callback<Ret(Args...)>& operator+=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    callback<Ret(Args...)>& operator=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and return connection for removing of the callback
    connection add(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<connectionData>(++highestID_);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return connection(*this, ptr);
    };

    //calls the callback
    std::vector<Ret> call(Args ... a)
    {
        auto vec = slots_; //if called functions manipulate callback

        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(auto& slot : vec)
            ret.push_back(slot.func(connectionRef(*this, slot.data), a ...));

        return ret;
    };

    //clears all registered callbacks and connections
    void clear()
    {
        slots_.clear();
    }

    std::vector<Ret> operator() (Args... a)
    {
        return call(a ...);
    }
};


//callback specialization for void because callback cant return a <void>-vector/////////////////////////////////
//callback////////////////////////////////////////////
template <class ... Args> class callback<void(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        std::shared_ptr<connectionData> data;
        std::function<void(const connectionRef&, Args ...)> func;
    };

protected:
    size_t highestID {0};
    std::vector<callbackSlot> slots_;

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
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

    //adds an callback by += operator
    callback<void(Args...)>& operator+=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    callback<void(Args...)>& operator=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and voidurn connection for removing of the callback
    connection add(compFunc<void(const connectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<connectionData>(++highestID);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return connection(*this, ptr);
    };

    //calls the callback
    void call(Args ... a)
    {
        auto vec = slots_; //if called functions manipulate callback

        for(auto& slot : vec)
            slot.func(connectionRef(*this, slot.data), a ...);
    };

    //clears all registered callbacks and connections
    void clear()
    {
        slots_.clear();
    }

    void operator() (Args... a)
    {
        call(a ...);
    }
};



//todo
//watcher classes as alternative to smart pointers
//before you use a object you can check if it is alive
//if an object dies it will signal all its watchers, so they know
/*
//base helper class watachable - better name?
class watchable
{
protected:
    callback<void()> destructionCallback_;

public:
    ~watchable()
    {
        destructionCallback_();
    }

    auto onDestruction(std::function<void()> func){ return destructionCallback_.add(func); }
};

//ref//////////////////////////////////////////////
//move, copy semntcs
template <typename T, typename B = typename std::conditional<std::is_base_of<watchable, T>::value, watchable, T>::type, std::unique_ptr<connection> (B::*Func)(std::function<void()>) = &B::onDestruction>
class watcherRef
{
protected:
    T* ref_;
    std::unique_ptr<connection> conn_ {nullptr};

public:
    ~watcherRef()
    {
        if(conn_) conn_->destroy();
    }

    T* get() const { return ref_; }
    void set(T& nref)
    {
        ref_ = &nref;
        conn_ = (nref.*Func)([=]{
            ref_ = nullptr;
            conn_.reset();
        });
    }
};
*/

}
