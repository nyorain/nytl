#pragma once

#include <nyutil/nonCopyable.hpp>
#include <nyutil/compFunc.hpp>

#include <functional>
#include <vector>
#include <algorithm>
#include <utility>
#include <mutex>
#include <memory>
#include <type_traits>

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
    virtual void remove(const connection& con) = 0;
    virtual void removeRef(const connectionRef& con) = 0;
    virtual void destroyed(const connection& con) = 0;
};


//connection//////////////////////////////////////////
class connection : public nonMoveable
{
protected:
    template<class T> friend class callback;
    template<class T> friend class tsafeCallback;

    callbackBase* callback_ {nullptr};

    explicit connection(callbackBase& call) : callback_(&call) {}
public:
    ~connection(){ if(callback_) callback_->destroyed(*this); }

    void destroy(){ if(callback_) callback_->remove(*this); callback_ = nullptr; }
    bool connected() const { return (callback_ != nullptr); }
};

//connectionRef for destroying a connection inside a callback////////////////////7
class connectionRef
{
protected:
    template<class T> friend class callback;
    template<class T> friend class tsafeCallback;

    mutable callbackBase* callback_ {nullptr};
    size_t id_;

    explicit connectionRef(callbackBase& call, size_t id) : callback_(&call), id_(id) {}
public:
    ~connectionRef() = default;

    connectionRef(const connectionRef& other) = default;
    connectionRef& operator=(const connectionRef& other) = default;
    //move would be the same

    void destroy() const { if(callback_) callback_->removeRef(*this); callback_ = nullptr; }
    bool connected() const { return (callback_ != nullptr); }
};


//threadsafe-callback////////////////////////////////////////////
template <class  Ret, class ... Args> class tsafeCallback<Ret(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        connection* con;
        std::function<Ret(const connectionRef&, Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;
    std::recursive_mutex mtx_; //too expensive for callback

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
    virtual void remove(const connection& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
                slots_.erase(it);
        }
    };

    //remove ref
    virtual void removeRef(const connectionRef& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        slots_.erase(slots_.begin() + con.id_);
    }

    //a connection which is destructed call this function
    //callback slot remains valid
    virtual void destroyed(const connection& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            if(it->con == &con)
            {
                it->con = nullptr;
                return;
            }
        }

    };

public:
    ~tsafeCallback()
    {
        clear();
    }

    //adds an callback by += operator
    tsafeCallback<Ret(Args...)>& operator+=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    tsafeCallback<Ret(Args...)>& operator=(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and return connection for removing of the callback
    std::unique_ptr<connection> add(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        auto c = std::unique_ptr<connection>(new connection(*this));

        std::lock_guard<std::recursive_mutex> lck(mtx_);
        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func.function();

        return c;
    };

    //calls the callback
    std::vector<Ret> call(Args ... a)
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);

        auto vec = slots_; //if called functions manipulate callback

        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(size_t i(0); i < slots_.size(); i++)
            ret.push_back(slots_[i].func(connectionRef(*this, i), a ...));

        return ret;
    };

    //clears all registered callbacks and connections
    void clear()
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto& s : slots_)
            if(s.con) s.con->callback_ = nullptr;

        slots_.clear();
    }

    std::vector<Ret> operator() (Args... a)
    {
        return call(a ...);
    }
};


//callback specialization for void because callback cant return a <void>-vector/////////////////////////////////
//callback////////////////////////////////////////////
template <class ... Args> class tsafeCallback<void(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        connection* con;
        std::function<void(const connectionRef&, Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;
    std::recursive_mutex mtx_;

    virtual void remove(const connection& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
            {
                slots_.erase(it);
                return;
            }
        }
    };

    virtual void removeRef(const connectionRef& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        slots_.erase(slots_.begin() + con.id_);
    }

    virtual void destroyed(const connection& con) override
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            if(it->con == &con)
            {
                it->con = nullptr;
                return;
            }
        }

    };

public:
    ~tsafeCallback()
    {
        clear();
    }

    //adds an callback by += operator
    tsafeCallback<void(Args...)>& operator+=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    tsafeCallback<void(Args...)>& operator=(compFunc<void(const connectionRef&, Args ...)> func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and voidurn connection for removing of the callback
    std::unique_ptr<connection> add(compFunc<void(const connectionRef&, Args ...)> func)
    {
        auto c = std::unique_ptr<connection>(new connection(*this));

        std::lock_guard<std::recursive_mutex> lck(mtx_);
        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func.function();

        return c;
    };

    //calls the callback
    void call(Args ... a)
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        auto vec = slots_; //if called functions manipulate callback

        for(size_t i(0); i < slots_.size(); i++)
            slots_[i].func(connectionRef(*this, i), a ...);
    };

    //clears all registered callbacks and connections
    void clear()
    {
        std::lock_guard<std::recursive_mutex> lck(mtx_);
        for(auto& s : slots_)
            if(s.con) s.con->callback_ = nullptr;

        slots_.clear();
    }

    void operator() (Args... a)
    {
        call(a ...);
    }
};





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//non-threadsafe callback/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class  Ret, class ... Args> class callback<Ret(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        connection* con;
        std::function<Ret(const connectionRef&, Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;

    virtual void remove(const connection& con) override
    {
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
                slots_.erase(it);
        }
    };

    virtual void removeRef(const connectionRef& con) override
    {
        slots_.erase(slots_.begin() + con.id_);
    }

    virtual void destroyed(const connection& con) override
    {
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            if(it->con == &con)
            {
                it->con = nullptr;
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
    std::unique_ptr<connection> add(compFunc<Ret(const connectionRef&, Args ...)> func)
    {
        auto c = std::unique_ptr<connection>(new connection(*this));

        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func.function();

        return c;
    };

    //calls the callback
    std::vector<Ret> call(Args ... a)
    {
        auto vec = slots_; //if called functions manipulate callback

        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(size_t i(0); i < slots_.size(); i++)
            ret.push_back(slots_[i].func(connectionRef(*this, i), a ...));

        return ret;
    };

    //clears all registered callbacks and connections
    void clear()
    {
        for(auto& s : slots_)
            if(s.con) s.con->callback_ = nullptr;

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
        connection* con;
        std::function<void(const connectionRef&, Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
    virtual void remove(const connection& con) override
    {
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
            {
                slots_.erase(it);
                return;
            }
        }
    };

    virtual void removeRef(const connectionRef& con) override
    {
        slots_.erase(slots_.begin() + con.id_);
    }

    virtual void destroyed(const connection& con) override
    {
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            if(it->con == &con)
            {
                it->con = nullptr;
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
    std::unique_ptr<connection> add(compFunc<void(const connectionRef&, Args ...)> func)
    {
        auto c = std::unique_ptr<connection>(new connection(*this));

        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func.function();

        return c;
    };

    //calls the callback
    void call(Args ... a)
    {
        auto vec = slots_; //if called functions manipulate callback

        for(size_t i(0); i < slots_.size(); i++)
            slots_[i].func(connectionRef(*this, i), a ...);
    };

    //clears all registered callbacks and connections
    void clear()
    {
        for(auto& s : slots_)
            if(s.con) s.con->callback_ = nullptr;

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
