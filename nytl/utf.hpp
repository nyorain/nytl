// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Small helper functions for using and converting utf-8 encoded strings.

#pragma once

#ifndef NYTL_INCLUDE_UTF_HPP
#define NYTL_INCLUDE_UTF_HPP

#include <string>
#include <array>
#include <locale>
#include <codecvt>
#include <stdexcept>

namespace nytl
{

///Returns the number of characters in a utf8-encoded unicode string.
///This differs from std::string::size because it does not return the bytes in the
///string, but the count of utf8-encoded characters.
inline std::size_t charCount(const std::string& utf8);

///Returns the nth chracter from a utf8-encoded unicode string.
///In difference to the builtin std::string access function this does not return
///the nth byte, but the nth utf8 character.
///Since every (unicode) utf8 character can take up to 4 bytes, an array holding
///4 chars is returned.
///\exception std::out_of_range if n > charCount(utf8)
inline std::array<char, 4> nth(const std::string& utf8, std::size_t n);

///\{
///Returns a reference to the nth character from a utf8-encoded unicode stirng.
///\param size [out] Will hold the number of bytes of the returned character.
///Will be not greater than 4.
///\exception std::out_of_range if n > charCount(utf8)
inline const char& nth(const std::string& utf8, std::size_t n, std::uint8_t& size);
inline char& nth(std::string& utf8, std::size_t n, std::uint8_t& size);
///\}

///\{
///Various conversion functions between different utf unicode encodings.
inline std::string toUtf8(const std::u16string& utf16);
inline std::string toUtf8(const std::u32string& utf32);
inline std::string toUtf8(const std::wstring& wide);
inline std::u16string toUtf16(const std::string& utf8);
inline std::u16string toUtf16(const std::u32string& utf32);
inline std::u16string toUtf16(const std::wstring& wide);
inline std::u32string toUtf32(const std::string& utf8);
inline std::u32string toUtf32(const std::u16string& utf16);
inline std::u32string toUtf32(const std::wstring& wide);
///\}

#include <nytl/bits/utf.inl>

}

#endif //header guard
