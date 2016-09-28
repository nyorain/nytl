// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_UTF_INL
#define NYTL_INCLUDE_UTF_INL

std::size_t charCount(const std::string& utf8)
{
	std::size_t count = 0u;
	for(auto& byte : utf8)
    	if((byte & 0xc0) != 0x80) ++count;

	return count;
}

std::array<char, 4> nth(const std::string& utf8, std::size_t n)
{
	std::array<char, 4> ret {};

	std::size_t count = 0u;
	std::size_t charNum = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			ret[charNum] = byte;
			++charNum;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!charNum) throw std::out_of_range("ny::nth(utf8)");
	return ret;
}

const char& nth(const std::string& utf8, std::size_t n, std::uint8_t& size)
{
	const char* ret = nullptr;
	size = 0;

	std::size_t count = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			if(size == 0) ret = &byte;
			++size;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!ret) throw std::out_of_range("ny::nth(utf8)");
	return *ret;
}

char& nth(std::string& utf8, std::size_t n, std::uint8_t& size)
{
	char* ret = nullptr;
	size = 0;

	std::size_t count = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			if(size == 0) ret = &byte;
			++size;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!ret) throw std::out_of_range("ny::nth(utf8)");
	return *ret;
}

//TODO: some functions (u16/u32) require 2 conversions.
//can this be done more efficiently with one?
std::string toUtf8(const std::u16string& utf16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.to_bytes(utf16);
}
std::string toUtf8(const std::u32string& utf32)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.to_bytes(utf32);
}
std::string toUtf8(const std::wstring& wide)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wide);
}
std::u16string toUtf16(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.from_bytes(utf8);
}
std::u16string toUtf16(const std::u32string& utf32)
{
	return toUtf16(toUtf8(utf32));
}
std::u16string toUtf16(const std::wstring& wide)
{
	return toUtf16(toUtf8(wide));
}
std::u32string toUtf32(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.from_bytes(utf8);
}
std::u32string toUtf32(const std::u16string& utf16)
{
	return toUtf32(toUtf8(utf16));
}
std::u32string toUtf32(const std::wstring& wide)
{
	return toUtf32(toUtf8(wide));
}
std::wstring toWide(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(utf8);
}
std::wstring toWide(const std::u16string& utf16)
{
	return toWide(toUtf8(utf16));
}
std::wstring toWide(const std::u32string& utf32)
{
	return toWide(toUtf8(utf32));
}

#endif //header guard
