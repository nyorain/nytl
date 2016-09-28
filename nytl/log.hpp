// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines functions to dynamically output to different streams.

#pragma once

#ifndef NYTL_INCLUDE_LOG_HPP
#define NYTL_INCLUDE_LOG_HPP

#include <nytl/misc.hpp>

#include <iostream>
#include <string>

namespace nytl
{

///Class to easily write output to a dynamically assignable stream.
class Logger
{
public:
    std::string prefix {};
    std::string name {"log"};
    std::ostream* stream {nullptr};

public:
    Logger() = default;
    Logger(std::ostream& os) : stream(&os){};
    Logger(const std::string& pre, const std::string& strm, std::ostream& str = std::cout)
		: prefix(pre), name(strm), stream(&str) {}

	///Outputs the given args to the ostream if valid.
    template<typename... Args> inline
    void output(Args&&... args) const
		{  if(stream) printVars(*stream, prefix, name, ": ", std::forward<Args>(args)..., "\n"); }

	///Operator wrapper for output().
    template<typename... Args> inline
    void operator()(Args&&... args) const
		{ output(std::forward<Args>(args)...); }
};

}

#endif //header guard
