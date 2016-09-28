// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Small utility header around std::chrono.

#pragma once

#ifndef NYTL_INCLUDE_TIME_HPP
#define NYTL_INCLUDE_TIME_HPP

#include <chrono>

///This enabled the chrono literals
using namespace std::literals::chrono_literals;

namespace nytl
{

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using TimeDuration = Clock::duration;

using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Seconds = std::chrono::seconds;
using Minutes = std::chrono::minutes;
using Hours = std::chrono::hours;

using std::chrono::duration_cast;
using std::chrono::time_point_cast;

///\ingroup utility
///Offers functionality to track the elapsed time since a set time point.
class Timer
{
public:
    TimePoint point = Clock::now();

public:
	///Resets the given timer.
    void reset(){ point = Clock::now(); };

	///Returns the timer since the last reset.
    TimeDuration elapsed() const { return Clock::now() - point; };
};

}

#endif //header guard
