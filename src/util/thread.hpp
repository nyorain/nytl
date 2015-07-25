#pragma once

#include <util/time.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

namespace util
{

//threadSafeObj//////////////////////////////////////////////////////
class threadSafeObj
{
protected:
    mutable std::mutex mutex_;
    mutable std::mutex lockMutex_;
    mutable bool locked_ = 0;
    mutable std::thread::id ownerID_;

public:
    threadSafeObj() = default;

    virtual void lock() const
    {
        std::unique_lock<std::mutex> lck(lockMutex_);
        if(!ownedByMe())
        {
            lck.unlock();
            mutex_.lock();
            lck.lock();
            locked_ = 1;
            ownerID_ = std::this_thread::get_id();
        }
    }

    virtual bool tryLock() const
    {
        std::unique_lock<std::mutex> lck(lockMutex_);
        if(!ownedByMe())
        {
            bool suc = mutex_.try_lock();

            if(suc)
            {
                locked_ = 1;
                ownerID_ = std::this_thread::get_id();
                return 1;
            }
        }
        return 0;
    }

    virtual void unlock() const
    {
        std::unique_lock<std::mutex> lck(lockMutex_);
        if(ownedByMe())
        {
            mutex_.unlock();
            locked_ = 0;
            ownerID_ = std::thread::id();
        }
    }

    bool isLocked() const { return locked_; };
    bool ownedByMe() const { return (locked_ && (std::this_thread::get_id() == ownerID_)); };
};


///////////////////////////////////////////////////////////
class threadpool;

class taskBase
{
public:
    taskBase(timePoint p = now()) : point(p) {}
    taskBase(timeDuration d) : point(d.then()) {}

    virtual ~taskBase(){}

    timePoint point;

    bool operator > (const taskBase& other) const { return (point > other.point); }
    bool operator < (const taskBase& other) const { return (point < other.point); }

    virtual void operator()(){};
    virtual void operator()(threadpool& pool){ (*this)(); }
};

//task///////////////////////////////////////////////////////////
class task : public taskBase
{
public:
    std::function<void()> func;

    task(std::function<void()> f, timeDuration d) : taskBase(d), func(f) {}
    task(std::function<void()> f = std::function<void()>(), timePoint t = now()) : taskBase(t), func(f) {}

    virtual void operator()(){ func(); }
};

//threadpool///////////////////////////////////////////////////////
class threadpool
{
protected:
    bool exiting_ : 1;
    bool finish_ : 1;
    bool takeTasks_ : 1;

    std::vector<std::thread> threads_;

    std::priority_queue<taskBase*, std::vector<taskBase*>, std::greater<taskBase*>> tasks_;
    std::condition_variable cv_;
    std::condition_variable cvDone_;
    std::mutex mtx_;

    //////////////////////////////////////////
    void threadFunc(size_t id)
    {
        std::unique_lock<std::mutex> lck(mtx_);
        taskBase* topTask = nullptr;

        while(!exiting_)
        {
            if(tasks_.empty())cv_.wait(lck, [this]{ return (this->exiting_ || !this->tasks_.empty()); });
            if(exiting_)
            {
                return;
            }

            topTask = tasks_.top();
            tasks_.pop();

            while(topTask->point.isInFuture())
            {
                cv_.wait_for(lck, topTask->point.timeFromNow());

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
            (*topTask)(*this);
            lck.lock();

            delete topTask;
            topTask = nullptr;

            if(finish_)
            {
                if(tasks_.empty()) cvDone_.notify_all();
            }
        }
    }

public:
    /////////////////////
    threadpool(size_t count): exiting_(0), finish_(0), takeTasks_(1)
    {
        for(size_t i(0); i < count; i++)
        {
            threads_.push_back(std::thread(&threadpool::threadFunc, this, i));
        }
    }

    //////////////////////
    ~threadpool()
    {
    std::unique_lock<std::mutex> lck(mtx_);

    while(!tasks_.empty())
    {
        delete tasks_.top();
        tasks_.pop();
    }

    exiting_ = 1;
    takeTasks_ = 0;
    cv_.notify_all();

    for(unsigned int i(0); i < threads_.size(); i++)
    {
        lck.unlock();

        if(threads_[i].joinable())
            threads_[i].join();

        lck.lock();
    }

    threads_.clear();
    }

    //////////////////////////////
    void addTask(taskBase* t)
    {
        std::unique_lock<std::mutex> lck(mtx_);

        if(!takeTasks_ || !t || exiting_)
            return;

        tasks_.push(t);
        cv_.notify_one();
    }

    /////////////////////////////
    void waitForFinish(bool noNewTasks = 0)
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
        cv_.notify_all();

        for(unsigned int i(0); i < threads_.size(); i++)
        {
            lck.unlock();

            if(threads_[i].joinable())
                threads_[i].join();

            lck.lock();
        }

        threads_.clear();
    }

    ////////////////////////////
    size_t taskCount() const { return tasks_.size(); }
};

}
