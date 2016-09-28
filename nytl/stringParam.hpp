// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defined the StringParam and SizedStringParam classes for efficient string paramters.

#pragma once

#ifndef NYTL_INCLUDE_STRINGPARAM_HPP
#define NYTL_INCLUDE_STRINGPARAM_HPP

#include <string>
#include <cstring>
#include <nytl/range.hpp>

namespace nytl
{

///This Overload class can be used to effeciently accept string parameters.
///Usually, there are 3 possibilities to take string params:
///- Take a const std::string& param (may introduce a huge overhead)
///- Take a const char* param (inconvinient to always call .c_str() on strings)
///- Overload the function for both versions (you have to declare 2 functions)
///Instead, one can simply take a const StringParam& parameter, which will introduce no
///additional overhead and easily constructs from const char* or a std::string.
///It can then (for usage) be converted into a const char*, but note that it will not be possible
///to get the length of the string later on. If you later need the size in any way, use
///SizedStringParam instead, which might me more efficient than later querying the length
///with std::strlen.
///\sa SizedStringParam
class StringParam
{
public:
	constexpr StringParam() noexcept = default;
	constexpr StringParam(const char* chars) noexcept : data_(chars) {}
	StringParam(const std::string& string) noexcept : data_(string.c_str()) {} 

	constexpr bool empty() const noexcept { return data_ == nullptr; }
	constexpr const char* data() const noexcept { return data_; }
	std::string string() const { return data_; }

	constexpr operator const char*() const noexcept { return data_; }

public:
	const char* data_ = nullptr;
};

inline bool operator==(const StringParam& param, const char* other) 
	{ return std::strcmp(param.data(), other) == 0; }
inline bool operator!=(const StringParam& param, const char* other) 
	{ return std::strcmp(param.data(), other) != 0; }

inline bool operator==(const StringParam& param, const std::string& other)
	{ return std::strcmp(param.data(), other.c_str()) == 0; }
inline bool operator!=(const StringParam& param, const std::string& other)
	{ return std::strcmp(param.data(), other.c_str()) != 0; }


///Class dervied from StringParam that also holds the length of the stored string.
///Can be used as parameter type for strings which size is needed in performance-critical
///functions.
///\sa StringParam
class SizedStringParam : public StringParam
{
public:
	constexpr SizedStringParam() noexcept = default;
	constexpr SizedStringParam(const char* chars, std::size_t length) noexcept
		: StringParam(chars), length_(length) {}
	constexpr SizedStringParam(const Range<char>& range) noexcept
		: StringParam(range.data()), length_(range.size()) {}

	SizedStringParam(const char* chars) noexcept
		: StringParam(chars), length_(std::strlen(chars)) {}
	SizedStringParam(const std::string& string) noexcept
		: StringParam(string), length_(string.size()) {}

	constexpr std::size_t length() const noexcept { return length_; }
	constexpr std::size_t size() const noexcept { return length_; }

	constexpr const char* begin() const noexcept { return data_; }
	constexpr const char* end() const noexcept { return data_ + length_; }

	constexpr const char* cbegin() const noexcept { return data_; }
	constexpr const char* cend() const noexcept { return data_ + length_; }

	constexpr operator const char*() const noexcept { return data_; }
	constexpr operator Range<char>() const noexcept { return {data_, length_}; }

public:
	std::size_t length_ = 0;
};

}

#endif //header guard
