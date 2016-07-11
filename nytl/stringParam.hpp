/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
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
///\brief Defined the StringParam and SizedStringParam classes for efficient string paramters.

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
	constexpr StringParam() = default;
	constexpr StringParam(const char* chars) : data_(chars) {}
	StringParam(const std::string& string) : data_(string.c_str())
		{ if(string.empty()) data_ = nullptr; } ///XXX: is this check needed/useful?

	constexpr bool empty() const { return data_ == nullptr; }
	constexpr const char* data() const { return data_; }

	constexpr operator const char*() const { return data_; }

public:
	const char* data_ = nullptr;
};

///Class dervied from StringParam that also holds the length of the stored string.
///Can be used as parameter type for strings which size is needed in performance-critical
///functions.
///\sa StringParam
class SizedStringParam : public StringParam
{
public:
	constexpr SizedStringParam() = default;
	SizedStringParam(const std::string& string) : StringParam(string), length_(string.size()) {}
	constexpr SizedStringParam(const char* chars) : StringParam(chars), length_(std::strlen(chars)) {}
	constexpr SizedStringParam(const Range<char>& range) : StringParam(range.data()),
		length_(range.size()) {}

	constexpr std::size_t length() const { return length_; }
	constexpr std::size_t size() const { return length_; }

	constexpr const char* begin() const { return data_; }
	constexpr const char* end() const { return data_ + length_; }

	constexpr const char* cbegin() const { return data_; }
	constexpr const char* cend() const { return data_ + length_; }

	constexpr operator const char*() const { return data_; }
	constexpr operator Range<char>() const { return {data_, length_}; }

public:
	std::size_t length_ = 0;
};

}
