#pragma once

#include <nyutil/vec.hpp>
#include <nyutil/time.hpp>

#include <set>

namespace nyutil
{

enum class operation
{
    Set,
    Add,
    Substract,
    Multiply,
    Divide
};

template<class A, class B> void performOperation(operation o, A& a, const B& b)
{
    switch(o)
    {
    case operation::Set:
        a = b;
        return;
    case operation::Add:
        a += b;
        return;
    case operation::Substract:
        a -= b;
        return;
    case operation::Multiply:
        a *= b;
        return;
    case operation::Divide:
        a /= b;
        return;
    default:
        return;
    }
}

template<class T> struct animationPoint
{
    animationPoint(double p,T v) : point(p), value(v) {}

    double point;
    T value;

    bool operator <(const animationPoint& other) const { return (point < other.point); }
};

template<class O> class animation
{
protected:
    std::set<animationPoint<O>> points_;
    O& value_;
    O originalValue_;

    timer timer_;
    timeDuration time_;

    operation operation_;

    bool running_;
public:
    animation(O& value, operation o = operation::Set) : value_(value), originalValue_(value), operation_(o) {};

    void addPoint(const animationPoint<O>& p){points_.insert(p); }
    void addPoint(double point, O value){points_.insert(animationPoint<O>(point, value)); }

    void setTime(timeDuration t){time_ = t; }

    void start(){ timer_.reset(); running_ = 1; }
    void stop(){ running_ = 0; }
    void reset(const O& value){ value_ = value; originalValue_ = value; } //does only reset the VALUE of the reference, not the reference itself because ... its a reference...
    void reset(){ value_ = originalValue_; }

    O& update()
    {
        if(!running_)
            return value_;

        if(timer_.getElapsedTime() > time_) //last point
        {
            typename std::set<animationPoint<O>>::reverse_iterator it = points_.rbegin();
            value_ = originalValue_;
            performOperation(operation_, value_, (*it).value);
            running_ = 0;
            return value_;
        }

        double currPoint = (double)timer_.getElapsedTime().asNanoseconds() / (double)time_.asNanoseconds();

        for(typename std::set<animationPoint<O>>::iterator it = points_.begin(); it != points_.end(); it++)
        {
            if((*it).point > currPoint)
            {
                if(it == points_.begin()) //first point
                {
                    value_ = originalValue_;
                    performOperation(operation_, value_, (*it).value);
                    return value_;
                }

                typename std::set<animationPoint<O>>::iterator before = it;
                before--;

                double lengthBetween = (*it).point - (*before).point;
                double higherFac = (currPoint - (*before).point) / lengthBetween;
                double lowerFac = ((*it).point - currPoint) / lengthBetween; //or 1 - lowerFac

                //std::cout <<  (*before).point << " " <<  (*it).point << " " << currPoint << " " << lowerFac << " " << higherFac << std::endl;

                O result = (lowerFac * (*before).value) + (higherFac * (*it).value);
                value_ = originalValue_;
                performOperation(operation_, value_, result);
                return value_;
            }
        }

        //should never occur
        running_ = 0;
        return value_;
    }

    bool running() const { return running_; }
    timeDuration getElapsedTime() const { if(running_)return timer_.getElapsedTime(); return time_; }

    O& getValue(){ return value_; }
    const O& getValue() const { return value_; }
};

template<class T, class O> class objAnimation : public animation<O>
{
protected:
    T object_;
    O T::*var_;

public:
    objAnimation(O T::*var, const T& obj = T(), operation type = operation::Set) : object_(obj), var_(var), animation<O>(object_.*var_) {}

    T& getObject() { return object_; };
    const T& getObject() const { return object_; };

    T& update(){ animation<O>::update(); return object_; }

    void reset(const T& obj){ object_ = obj; animation<O>::reset(object_.*var_); }
    void reset(const O& val){ animation<O>::reset(val); }
    void reset(){ animation<O>::reset(); }
};


}
