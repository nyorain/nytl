#pragma once

#include <nyutil/nonCopyable.hpp>
#include <nyutil/slist.hpp>

#include <functional>
#include <vector>
#include <algorithm>
#include <utility>
#include <mutex>
#include <memory>


namespace nyutil
{

class connection;
class slot;
template < class > class callback;

//class callback should be thread safe
//atm implemented using expensive mutexes etc., should change to lock free containers (deque/queue) later on

//callbackBase//////////////////////////////////////
class callbackBase
{

friend class connection;

protected:
    virtual void remove(const connection& con) = 0;
    virtual void destroyed(const connection& con) = 0;
};

//connection//////////////////////////////////////////
class connection : public nonCopyable
{
protected:
    template<class T> friend class callback;
    callbackBase* callback_ { nullptr };

public:
    connection(callbackBase& call) : callback_(&call) {}
    ~connection(){ if(callback_) callback_->destroyed(*this); }

    void destroy(){ if(callback_) callback_->remove(*this); callback_ = nullptr; }
    bool connected() const { return (callback_ != nullptr); }
};

//callback////////////////////////////////////////////
template <class  Ret, class ... Args> class callback<Ret(Args...)> final : public callbackBase
{
protected:
    struct callbackSlot
    {
        connection* con;
        std::function<Ret(Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;
    std::mutex mtx_; //too expensive for callback

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
    virtual void remove(const connection& con) override
    {
        std::lock_guard<std::mutex> lck(mtx_);
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
                slots_.erase(it);
        }
    };

    virtual void destroyed(const connection& con) override
    {
        std::lock_guard<std::mutex> lck(mtx_);
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
    callback<Ret(Args...)>& operator+=(const std::function<Ret(Args...)>& func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    callback<Ret(Args...)>& operator=(const std::function<Ret(Args...)>& func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and return connection for removing of the callback
    std::unique_ptr<connection> add(const std::function<Ret(Args ...)>& func)
    {
        auto c = std::make_unique<connection>(*this);

        std::lock_guard<std::mutex> lck(mtx_);
        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func;

        return c;
    };

    //calls the callback
    std::vector<Ret> call(Args ... a)
    {
        std::lock_guard<std::mutex> lck(mtx_);
        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(auto& s : slots_)
            ret.push_back(s.func(a ...));

        return ret;
    };

    //clears all registered callbacks and connections
    void clear()
    {
        std::lock_guard<std::mutex> lck(mtx_);
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
        std::function<void(Args ...)> func;
    };

protected:
    std::vector<callbackSlot> slots_;
    std::mutex mtx_; //too expensive for callback

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
    virtual void remove(const connection& con) override
    {
        std::lock_guard<std::mutex> lck(mtx_);
        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->con == &con)
            {
                slots_.erase(it);
                return;
            }
        }
    };

    virtual void destroyed(const connection& con) override
    {
        std::lock_guard<std::mutex> lck(mtx_);
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
    callback<void(Args...)>& operator+=(const std::function<void(Args...)>& func)
    {
        add(func);
        return *this;
    };

    //clears all callbacks and sets one new callback
    callback<void(Args...)>& operator=(const std::function<void(Args...)>& func)
    {
        clear();
        add(func);
        return *this;
    };

    //adds new callback and voidurn connection for removing of the callback
    std::unique_ptr<connection> add(const std::function<void(Args ...)>& func)
    {
        auto c = std::make_unique<connection>(*this);

        std::lock_guard<std::mutex> lck(mtx_);
        slots_.emplace_back();

        slots_.back().con = c.get();
        slots_.back().func = func;

        return c;
    };

    //calls the callback
    void call(Args ... a)
    {
        std::lock_guard<std::mutex> lck(mtx_);

        for(auto& s : slots_)
            s.func(a ...);
    };

    //clears all registered callbacks and connections
    void clear()
    {
        std::lock_guard<std::mutex> lck(mtx_);
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
