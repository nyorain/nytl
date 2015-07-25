#pragma once

#include <nyutil/nonCopyable.hpp>

#include <functional>
#include <vector>
#include <algorithm>
#include <utility>


namespace nyutil
{

class connection;
template < class > class callback;

//class callbackBase//////////////////////////////////////
class callbackBase
{
public:
    virtual void remove(const connection& con) = 0;
};

//connection//////////////////////////////////////////
class connection : public nonCopyable
{
protected:
    template<class T> friend class callback;

    callbackBase& callback_;
    bool onlyOnce_ = 0;

public:
    connection(callbackBase& call, bool onlyOnce = 0) : callback_(call), onlyOnce_(onlyOnce) {}
    virtual ~connection(){}

    connection(const connection&& mover) noexcept : callback_(mover.callback_), onlyOnce_(mover.onlyOnce_) {} //for callback
    connection& operator=(const connection&& mover) noexcept { callback_ = mover.callback_; onlyOnce_ = mover.onlyOnce_; return *this; } //for callback

    bool onlyOnce() const { return onlyOnce_; };
    void setOnlyOnce(bool s = 1){ onlyOnce_ = s; }

    void destroy(){ callback_.remove(*this); } //will delete this object implicitly
};



//callback////////////////////////////////////////////
template <class  Ret, class ... Args> class callback<Ret(Args...)> : public callbackBase
{
protected:
    std::vector< std::pair<connection, std::function<Ret(Args ...)>> > callbacks_;

public:

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
    connection& add(const std::function<Ret(Args ...)>& func, bool onlyOnce = 0)
    {
        connection conn(*this, onlyOnce);

        callbacks_.push_back(std::make_pair(std::move(conn), func));

        return callbacks_.back().first;
    };

    //removes a callback identified by its connection. Functions (std::function) can't be compared => we need connections
    void remove(const connection& con)
    {
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            if(&callbacks_[i].first == &con)
            {
                callbacks_.erase(callbacks_.begin() + i);
                return;
            }
        }
    };

    //calls the callback
    std::vector<Ret> call(Args ... a)
    {
        std::vector<Ret> ret;
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            ret.push_back(callbacks_[i].second(a ...));
            if(callbacks_[i].first.onlyOnce())
                remove(callbacks_[i].first);
        }
        return ret;
    };

    //clears all registered callbacks and connections
    void clear()
    {
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            callbacks_[i].first.wasRemoved();
        }

        callbacks_.clear();
    }

    std::vector<Ret> operator() (Args... a)
    {
        return call(a ...);
    }
};


//callback specialization for void because callback cant return a <void>-vector/////////////////////////////////
template< class ... Args> class callback<void(Args...)> : public callbackBase
{
protected:
    std::vector< std::pair<connection, std::function<void(Args ...)>> > callbacks_;

public:
    callback<void(Args...)>& operator+=(const std::function<void(Args...)>& func)
    {
        add(func);
        return *this;
    };

    callback<void(Args...)>& operator=(const std::function<void(Args...)>& func)
    {
        clear();
        add(func);
        return *this;
    };

    connection& add(const std::function<void(Args...)>& func, bool onlyOnce = 0)
    {
        connection conn(*this, onlyOnce);

        callbacks_.push_back(std::make_pair(std::move(conn), func));
        return callbacks_.back().first;
    };

    void remove(const connection& con)
    {
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            if(&callbacks_[i].first == &con)
            {
                callbacks_.erase(callbacks_.begin() + i);
                return;
            }
        }
    };

    void call(Args ... a)
    {
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            callbacks_[i].second(a ...);
            if(callbacks_[i].first.onlyOnce())
                remove(callbacks_[i].first);
        }
    };

    void clear()
    {
        for(unsigned int i(0); i < callbacks_.size(); i++)
        {
            callbacks_[i].first.wasRemoved();
        }

        callbacks_.clear();
    }

    void operator() (Args... a)
    {
        call(a ...);
    }
};

}
