// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Small helper functions for using and converting utf-8 encoded strings.

#pragma once

#ifndef NYTL_INCLUDE_UTF_HPP
#define NYTL_INCLUDE_UTF_HPP

#include <string> // std::string
#include <array> // std::array
#include <locale> // std::wstring_convert
#include <codecvt> // std::codecvt_utf8
#include <stdexcept> // std::out_of_range

namespace nytl {

/// \brief Returns the number of characters in a utf8-encoded unicode string.
/// This differs from std::string::size because it does not return the bytes in the
/// string, but the count of utf8-encoded characters.
/// \module utf
std::size_t charCount(const std::string& utf8)
{
	std::size_t count = 0u;
	for(auto& byte : utf8)
		if((byte & 0xc0) != 0x80) ++count;

	return count;
}

/// \brief Returns the nth chracter from a utf8-encoded unicode string.
/// In difference to the builtin std::string access function this does not return
/// the nth byte, but the nth utf8 character.
/// Since every (unicode) utf8 character can take up to 4 bytes, an array holding
/// 4 chars is returned.
/// \throws std::out_of_range if n > charCount(utf8)
/// \module utf
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

/// Returns a reference to the nth character from a utf8-encoded unicode stirng.
/// \param size [out] Will hold the number of bytes of the returned character.
/// Will be not greater than 4.
/// \throws std::out_of_range if n > charCount(utf8)
/// \module utf
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

/// Returns a reference to the nth character from a utf8-encoded unicode stirng.
/// \param size [out] Will hold the number of bytes of the returned character.
/// Will be not greater than 4.
/// \throws std::out_of_range if n > charCount(utf8)
/// \module utf
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

/// \brief Converts the given utf16 string to a utf8 string.
/// \module utf
std::string toUtf8(const std::u16string& utf16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.to_bytes(utf16);
}

/// \brief Converts the given utf32 string to a utf8 string.
/// \module utf
std::string toUtf8(const std::u32string& utf32)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.to_bytes(utf32);
}

/// \brief Converts the given utf8 string to a utf16 string.
/// \module utf
std::u16string toUtf16(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.from_bytes(utf8);
}

/// \brief Converts the given utf32 string to a utf16 string.
/// \module utf
std::u16string toUtf16(const std::u32string& utf32)
{
	return toUtf16(toUtf8(utf32));
}

/// \brief Converts the given utf8 string to a utf32 string.
/// \module utf
std::u32string toUtf32(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.from_bytes(utf8);
}

/// \brief Converts the given utf16 string to a utf32 string.
/// \module utf
std::u32string toUtf32(const std::u16string& utf16)
{
	return toUtf32(toUtf8(utf16));
}

} // namespace nytl

#endif // header guard
