// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_STRING_PARAM
#define NYTL_INCLUDE_STRING_PARAM

#include <string_view>
#include <string>

namespace nytl {

/// Like std::string_view but guaranteed to be null-terminated.
/// Makes it actually useful as string type parameter.
class StringParam : public std::string_view {
public:
	constexpr StringParam() = default;
	constexpr StringParam(const char* cstr) : std::string_view(cstr) {}
	StringParam(const std::string& str) : std::string_view(str.c_str()) {}

	const char* c_str() const { return data(); }
};

} // namespace nytl