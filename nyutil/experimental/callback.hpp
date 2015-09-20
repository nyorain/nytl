#pragma once

#include <nyutil/nonCopyable.hpp>
#include <nyutil/compFunc.hpp>
#include <nyutil/callback.hpp>

#include <functional>
#include <vector>
#include <algorithm>
#include <utility>
#include <mutex>
#include <memory>
#include <type_traits>

namespace nyutil
{

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


}
