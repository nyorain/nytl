#pragma once

#include <nytl/time.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <queue>
#include <future>
#include <functional>

namespace nytl
{

using mtxGuard = std::lock_guard<std::mutex>;
using mtxLock = std::unique_lock<std::mutex>;

//threadSafeObj//////////////////////////////////////////////////////
class threadSafeObj
{
protected:
    mutable std::recursive_mutex mutex_;

public:
    threadSafeObj() = default;

    void lock() const { mutex_.lock(); }
    bool try_lock() const { return mutex_.try_lock(); }
    void unlock() const { mutex_.unlock(); }
};

//msgThread
template <typename Msg>
class msgThread
{
protected:
    std::deque<Msg> msgs_ {};
    std::condition_variable msgCv_ {};
    std::mutex msgMtx_ {};
    std::thread thread_ {};
    std::atomic<bool> exit_{0};
    std::function<bool(const Msg&)> msgCallback_{};

    void run()
    {
        while(1)
        {
            Msg msg{};

            {
                mtxLock lck(msgMtx_);
                while(msgs_.empty() && !exit_.load())
                    msgCv_.wait(lck);

                if(exit_.load())
                    return;

                msg = std::move(msgs_.front());
                msgs_.pop_front();
            }

            if(!msgCallback_(msg))
            {
                exit_.store(1);
                return;
            }
        }
    }

public:
    msgThread(std::function<bool(const Msg&)> msgCB) : thread_(&msgThread::run, this), msgCallback_(msgCB)
    {
    }

    virtual ~msgThread()
    {
        if(!exit_.load())
        {
            exit_.store(true);
            msgCv_.notify_one();
        }

        thread_.join();
    }

    bool sendMessage(const Msg& msg)
    {
        {
            mtxGuard lck(msgMtx_);
            msgs_.push_back(msg);
        }

        msgCv_.notify_one();
        return exit_.load();
    }

    bool sendMessage(Msg&& msg)
    {
        {
            mtxGuard lck(msgMtx_);
            msgs_.emplace_back(std::move(msg));
        }

        msgCv_.notify_one();
        return exit_.load();
    }

    bool running() const { return exit_.load(); }
};

//threadpool///////////////////////////////////////////////////////
class threadpool
{
protected:
    //taskBase
    struct task
    {
        virtual ~task() = default;

        timePoint point;
        virtual void operator()() = 0;
    };

    //template impl
    template<typename Ret> struct taskImpl : public task
    {
        std::function<Ret()> func;
        std::promise<Ret> promise;

        virtual void operator()() override
        {
            try
            {
                promise.set_value(func());
            }
            catch(...)
            {
                promise.set_exception(std::current_exception());
            }

        }
    };

    //compare for pq
    struct taskGreater
    {
        bool operator()(task* a, task* b) const
        {
            return a->point > b->point;
        }
    };

protected:
    bool exiting_ : 1;
    bool finish_ : 1;
    bool takeTasks_ : 1;

    std::vector<std::thread> threads_;

    std::priority_queue<task*, std::vector<task*>, taskGreater> tasks_;
    std::condition_variable cvAdd_;
    std::condition_variable cvDone_;
    std::mutex mtx_;

    inline void threadFunc(size_t id);

public:
    inline threadpool();
    inline threadpool(size_t count);
    inline ~threadpool();

    template<typename F> auto addTask(F func, timeDuration td) -> std::future<typename std::result_of<F()>::type>;
    template<typename F> auto addTask(F func, timePoint tp = now()) -> std::future<typename std::result_of<F()>::type>;

    inline void waitForFinish(bool noNewTasks = 0);
    inline size_t taskCount() const;
    inline size_t size() const;
};

//specialization void
template<> struct threadpool::taskImpl<void> : public task
{
    std::function<void()> func;
    std::promise<void> promise;

    virtual void operator()() override
    {
        try
        {
            func();
            promise.set_value();
        }
        catch(...)
        {
            promise.set_exception(std::current_exception());
        }
    }
};

//impl//////////////////////////////////////////////////
threadpool::threadpool() : exiting_(0), finish_(0), takeTasks_(1)
{
    for(size_t i(0); i < std::thread::hardware_concurrency(); i++)
    {
        threads_.push_back(std::thread(&threadpool::threadFunc, this, i));
    }
}

threadpool::threadpool(size_t count) : exiting_(0), finish_(0), takeTasks_(1)
{
    for(size_t i(0); i < count; i++)
    {
        threads_.push_back(std::thread(&threadpool::threadFunc, this, i));
    }
}

threadpool::~threadpool()
{
    mtxLock lck(mtx_);
    while(!tasks_.empty())
    {
        delete tasks_.top();
        tasks_.pop();
    }

    exiting_ = 1;
    takeTasks_ = 0;
    cvAdd_.notify_all();

    for(unsigned int i(0); i < threads_.size(); i++)
    {
        lck.unlock();

        if(threads_[i].joinable())
            threads_[i].join();

        lck.lock();
    }

    threads_.clear();
}

template <typename F> auto threadpool::addTask(F func, timeDuration time) -> std::future<typename std::result_of<F()>::type>
{
    auto t = new taskImpl<typename std::result_of<F()>::type>();
    t->point = time.then();
    t->func = func;

    auto fut = t->promise.get_future();

    {
        mtxGuard lck(mtx_);
        tasks_.push(t);
    }

    cvAdd_.notify_one();
    return fut;
}

template <typename F> auto threadpool::addTask(F func, timePoint point) -> std::future<typename std::result_of<F()>::type>
{
    auto t = new taskImpl<typename std::result_of<F()>::type>();
    t->point = point;
    t->func = func;

    auto fut = t->promise.get_future();

    {
        mtxGuard lck(mtx_);
        tasks_.push(t);
    }

    cvAdd_.notify_one();
    return fut;
}

void threadpool::threadFunc(size_t id)
{
    std::unique_lock<std::mutex> lck(mtx_);

    while(!exiting_)
    {
        if(tasks_.empty())cvAdd_.wait(lck, [this]{ return (this->exiting_ || !this->tasks_.empty()); });
        if(exiting_)
        {
            return;
        }

        auto topTask = tasks_.top();
        tasks_.pop();

        while(topTask->point.isInFuture())
        {
            cvAdd_.wait_for(lck, topTask->point.timeFromNow());

            if(exiting_)
            {
                if(!finish_)
                    return;
            }

            if(topTask->point > tasks_.top()->point) //kleiner == before, x > y -> x nach y
            {
                tasks_.push(topTask);
                topTask = tasks_.top();
                tasks_.pop();
            }
        }

        lck.unlock();
        (*topTask)();

        delete topTask;
        topTask = nullptr;

        lck.lock();
        if(finish_ && tasks_.empty())
        {
            cvDone_.notify_all();
        }
   }
}

void threadpool::waitForFinish(bool noNewTasks)
{
    std::unique_lock<std::mutex> lck(mtx_);
    finish_ = 1;

    if(noNewTasks)
        takeTasks_ = 0;

    while(1)
    {
        cvDone_.wait(lck);
        if(tasks_.empty())
            break;
    }


    exiting_ = 1;
    cvAdd_.notify_all();

    for(unsigned int i(0); i < threads_.size(); i++)
    {
        lck.unlock();

        if(threads_[i].joinable())
            threads_[i].join();

        lck.lock();
    }

    threads_.clear();
}

size_t threadpool::taskCount() const
{
    return tasks_.size();
}

size_t threadpool::size() const
{
    return threads_.size();
}

}
