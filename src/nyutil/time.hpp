#pragma once

#include <chrono>
#include <ratio>

using namespace std::literals::chrono_literals;

namespace nyutil
{

class timePoint;

typedef std::chrono::duration<double,std::ratio<1,1000000000>> nanoseconds;
typedef std::chrono::duration<double,std::ratio<1,1000000>> microseconds;
typedef std::chrono::duration<double,std::ratio<1,1000>> milliseconds;
typedef std::chrono::duration<double,std::ratio<1,1>> seconds;
typedef std::chrono::duration<double,std::ratio<60,1>> minutes;
typedef std::chrono::duration<double,std::ratio<3600,1>> hours;

class timeDuration : public std::chrono::high_resolution_clock::duration
{
protected:
    typedef std::chrono::high_resolution_clock::duration stdDuration;
public:
    timeDuration() = default;
    timeDuration(const stdDuration& other) : stdDuration(other) {}
    template<class T, class R> timeDuration(const std::chrono::duration<T, R>& other) :stdDuration(std::chrono::duration_cast<stdDuration>(other)) {}

    timeDuration& operator=(const stdDuration& other){ stdDuration::operator=(other); return *this; }
    template<class T, class R> timeDuration& operator=(const std::chrono::duration<T, R>& other){ stdDuration::operator=(other); return *this; }

    double asNanoseconds() const { return std::chrono::duration_cast<duration<double,std::ratio<1,1000000000>>>(*this).count(); }
    double asMicroseconds() const { return std::chrono::duration_cast<duration<double,std::ratio<1,1000000>>>(*this).count(); }
    double asMilliseconds() const { return std::chrono::duration_cast<duration<double,std::ratio<1,1000>>>(*this).count(); }
    double asSeconds() const { return std::chrono::duration_cast<duration<double,std::ratio<1,1>>>(*this).count(); }
    double asMinutes() const { return std::chrono::duration_cast<duration<double,std::ratio<60,1>>>(*this).count(); }
    double asHours() const { return std::chrono::duration_cast<duration<double,std::ratio<3600,1>>>(*this).count(); }

    timePoint then() const;
};

/*
inline timeDuration nanoseconds(long i){ return std::chrono::nanoseconds(i); };
inline timeDuration microseconds(long i){ return std::chrono::microseconds(i); };
inline timeDuration milliseconds(long i){ return std::chrono::milliseconds(i); };
inline timeDuration seconds(long i){ return std::chrono::seconds(i); };
inline timeDuration hours(long i){ return std::chrono::hours(i); };
*/

class timePoint : public std::chrono::high_resolution_clock::time_point
{
protected:
    typedef std::chrono::high_resolution_clock::time_point stdPoint;
public:
    timePoint() : stdPoint(std::chrono::high_resolution_clock::now()) {};
    timePoint(const timeDuration& d) : stdPoint(std::chrono::high_resolution_clock::now() + d) {};
    timePoint(const stdPoint& other) : stdPoint(other) {}
    template<class T, class R> timePoint(const std::chrono::time_point<T, R>& other) : stdPoint(std::chrono::time_point_cast<stdPoint>(other)) {}

    timePoint& operator=(const stdPoint& other){ stdPoint::operator=(other); return *this; }
    template<class T, class R> timePoint& operator=(const std::chrono::time_point<T, R>& other){ stdPoint::operator=(other); return *this; }

    timeDuration timeFromNow() const { return *this - timePoint(); }
    bool isInFuture() const { return timeDuration(*this - timePoint()).asNanoseconds() > 0; }

    void setFromNow(const timeDuration& d){ stdPoint::operator=(std::chrono::high_resolution_clock::now() + d); }
};

inline timePoint now(){ return timePoint(); }
inline timePoint timeDuration::then() const { return timePoint(*this); }

class timer
{
protected:
    timePoint last;

public:
    timer() = default;

    void reset(){ last = now(); };
    timeDuration getElapsedTime() const { return now() - last; };
};

}
