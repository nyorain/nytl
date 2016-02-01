#pragma once

#include <nytl/vec.hpp>
#include <nytl/time.hpp>

#include <set>

namespace nytl
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

template<class T> struct aniMationPoint
{
    aniMationPoint(double p,T v) : point(p), value(v) {}

    double point;
    T value;

    bool operator <(const aniMationPoint& other) const { return (point < other.point); }
};

template<class O> class aniMation
{
protected:
    std::set<aniMationPoint<O>> points_;
    O& value_;
    O originalvalue_type_;

    Timer Timer_;
    TimeDuration time_;

    operation operation_;

    bool running_;
public:
    aniMation(O& value, operation o = operation::Set) : value_(value), originalvalue_type_(value), operation_(o) {};

    void addPoint(const aniMationPoint<O>& p){points_.insert(p); }
    void addPoint(double point, O value){points_.insert(aniMationPoint<O>(point, value)); }

    void setTime(TimeDuration t){time_ = t; }

    void start(){ Timer_.reset(); running_ = 1; }
    void stop(){ running_ = 0; }
    void reset(const O& value){ value_ = value; originalvalue_type_ = value; } //does only reset the VALUE of the reference, not the reference itself because ... its a reference...
    void reset(){ value_ = originalvalue_type_; }

    O& update()
    {
        if(!running_)
            return value_;

        if(Timer_.getElapsedTime() > time_) //last point
        {
            typename std::set<aniMationPoint<O>>::reverse_iterator it = points_.rbegin();
            value_ = originalvalue_type_;
            performOperation(operation_, value_, (*it).value);
            running_ = 0;
            return value_;
        }

        double currPoint = (double)Timer_.getElapsedTime().asNanoseconds() / (double)time_.asNanoseconds();

        for(typename std::set<aniMationPoint<O>>::iterator it = points_.begin(); it != points_.end(); it++)
        {
            if((*it).point > currPoint)
            {
                if(it == points_.begin()) //first point
                {
                    value_ = originalvalue_type_;
                    performOperation(operation_, value_, (*it).value);
                    return value_;
                }

                typename std::set<aniMationPoint<O>>::iterator before = it;
                before--;

                double lengthBetween = (*it).point - (*before).point;
                double higherFac = (currPoint - (*before).point) / lengthBetween;
                double lowerFac = ((*it).point - currPoint) / lengthBetween; //or 1 - lowerFac

                //std::cout <<  (*before).point << " " <<  (*it).point << " " << currPoint << " " << lowerFac << " " << higherFac << std::endl;

                O result = (lowerFac * (*before).value) + (higherFac * (*it).value);
                value_ = originalvalue_type_;
                performOperation(operation_, value_, result);
                return value_;
            }
        }

        //should never occur
        running_ = 0;
        return value_;
    }

    bool running() const { return running_; }
    TimeDuration getElapsedTime() const { if(running_)return Timer_.getElapsedTime(); return time_; }

    O& getvalue_type(){ return value_; }
    const O& getvalue_type() const { return value_; }
};

template<class T, class O> class objAniMation : public aniMation<O>
{
protected:
    T object_;
    O T::*var_;

public:
    objAniMation(O T::*var, const T& obj = T(), operation type = operation::Set) : object_(obj), var_(var), aniMation<O>(object_.*var_) {}

    T& getObject() { return object_; };
    const T& getObject() const { return object_; };

    T& update(){ aniMation<O>::update(); return object_; }

    void reset(const T& obj){ object_ = obj; aniMation<O>::reset(object_.*var_); }
    void reset(const O& val){ aniMation<O>::reset(val); }
    void reset(){ aniMation<O>::reset(); }
};


}
