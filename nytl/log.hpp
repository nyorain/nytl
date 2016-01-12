/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Static stream objects which can e.g. be used for logging, warning or error output.  

#pragma once

#include <nytl/misc.hpp>

#include <iostream>
#include <string>

namespace nytl
{

//
class logStream
{
public:
    std::string prefix {""};
    std::string name {"log"};
    std::ostream* stream {&std::cout};
    bool active {1};

public:
    logStream() = default;
    logStream(bool actv) : active(actv) {};
    logStream(std::ostream& os, bool actv = 1) : stream(&os), active(actv) {};
    logStream(const std::string& pre, const std::string& strm, std::ostream& str = std::cout, 
			bool actv = 1) : prefix(pre), name(strm), stream(&str), active(actv) {}

    template<typename... Args> inline
    void output(Args... args) const 
	{ 
		if(!active || !stream) return; 
		printVars(*stream, prefix, name, ": ", args..., "\n"); 
	}

    template<typename... Args> inline
    void operator()(Args... args) const { output(args...); }
};

//
namespace detail
{
//debug
#ifndef NDEBUG
    constexpr bool dEnabled = 0;
#else
    constexpr bool dEnabled = 1;
#endif // NDEBUG

//init
inline logStream& initDebug()
	{ static logStream str{"", "debug", std::clog, detail::dEnabled}; return str; }
inline logStream& initLog()
	{ static logStream str{"", "log", std::clog}; return str; }
inline logStream& initWarning()
	{ static logStream str{"", "warning", std::cout}; return str; }
inline logStream& initError()
	{ static logStream str{"", "warning", std::cout}; return str; }

}

//global
static logStream& sendDebug = detail::initDebug();
static logStream& sendLog = detail::initLog();
static logStream& sendWarning = detail::initWarning();
static logStream& sendError = detail::initError();

}
