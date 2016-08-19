//The MIT License (MIT)
//
//Copyright (c) 2016 nyorain
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

///\file
///\brief Small utility header around std::chrono.

#pragma once

#ifndef NYTL_INCLUDE_TIME_HPP
#define NYTL_INCLUDE_TIME_HPP

#include <chrono>
#include <ratio>

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
