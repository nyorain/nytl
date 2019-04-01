// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Small helper functions for using and converting utf-8 encoded strings.

#pragma once

#ifndef NYTL_INCLUDE_UTF
#define NYTL_INCLUDE_UTF

#include <string> // std::string
#include <string_view> // std::string_view
#include <array> // std::array
#include <locale> // std::wstring_convert
#include <codecvt> // std::codecvt_utf8
#include <stdexcept> // std::out_of_range

// all operations assume correct utf8 string and don't perform any sanity checks.
// for implementation details see https://en.wikipedia.org/wiki/UTF-8

namespace nytl {

/// \brief Returns the number of characters in a utf8-encoded unicode string.
/// This differs from std::string::size because it does not return the bytes in the
/// string, but the count of utf8-encoded characters.
inline std::size_t charCount(std::string_view utf8) {
	std::size_t count = 0u;
	auto it = utf8.begin();

	while(it != utf8.end()) {
		auto length = 1u;
		if((*it) & (1 << 7)) {
			++length;
			if((*it) & (1 << 5)) {
				++length;
				if((*it) & (1 << 4))
					++length;
			}
		}

		++count;
		it += length;
	}

	return count;
}

/// \brief Returns the character at position n (started at 0) from the given utf8 string.
/// \details In difference to the builtin std::string access function this does not return
/// the nth byte, but the nth utf8 character.
/// Since every (unicode) utf8 character can take up to 4 bytes, the last char in the
/// returned array will always be '\0', which allows to treat its data as null-terminated
/// const char pointer.
/// Example: `nytl::nth(u8"äüß", 1)` returns the char "ü" as utf8 array,
/// i.e. {0xc3, 0xbc ,0, 0} since u8"ü"[0] == 0xc3 and u8"ü"[1] == 0xbc.
/// \note Indexing starts at zero! `nth(utf8, 1)` returns actually the second char.
/// \throws std::out_of_range if n > charCount(utf8)
inline std::array<char, 5> nth(std::string_view utf8, std::size_t n) {
	std::size_t count = 0u;
	auto it = utf8.begin();

	while(it != utf8.end()) {
		auto length = 1u;
		if((*it) & (1 << 7)) {
			++length;
			if((*it) & (1 << 5)) {
				++length;
				if((*it) & (1 << 4))
					++length;
			}
		}

		if(count == n) {
			std::array<char, 5> ret {};
			for(auto i = 0u; i < length; ++i) ret[i] = *(it + i);
			return ret;
		}

		++count;
		it += length;
	}

	throw std::out_of_range("ny::nth(utf8, n)");
}

/// \brief Retrieves the character at position n (started at 0) of the given utf8 string.
/// \returns A reference to the character at position n which can together with the
/// retrieved byte size of the requested character be used to read it.
/// \param size [out] Will hold the number of bytes of the requested character.
/// Will be not greater than 4 for a valid utf8 string.
/// \note Indexing starts at zero! `nth(utf8, 1)` returns actually the second char.
/// \throws std::out_of_range if n > charCount(utf8)
inline const char& nth(std::string_view utf8, std::size_t n, std::uint8_t& size) {
	auto count = 0u;
	auto it = utf8.begin();

	while(it != utf8.end()) {
		auto length = 1u;
		if((*it) & (1 << 7)) {
			++length;
			if((*it) & (1 << 5)) {
				++length;
				if((*it) & (1 << 4))
					++length;
			}
		}

		if(count == n) {
			size = length;
			return *it;
		}

		it += length;
		++count;
	}

	size = 0u;
	throw std::out_of_range("ny::nth(const utf8, n, size)");
}

/// \brief Retrieves the character at position n (started at 0) of the given utf8 string.
/// \returns A reference to the character at position n which can together with the
/// retrieved byte size of the requested character be used to read it.
/// \param size [out] Will hold the number of bytes of the requested character.
/// Will be not greater than 4.
/// \note Indexing starts at zero! `nth(utf8, 1)` returns actually the second char.
/// \throws std::out_of_range if n > charCount(utf8)
inline char& nth(std::string& utf8, std::size_t n, std::uint8_t& size) {
	auto count = 0u;
	auto it = utf8.begin();

	while(it != utf8.end()) {
		auto length = 1u;
		if((*it) & (1 << 7)) {
			++length;
			if((*it) & (1 << 5)) {
				++length;
				if((*it) & (1 << 4))
					++length;
			}
		}

		if(count == n) {
			size = length;
			return *it;
		}

		it += length;
		++count;
	}

	size = 0u;
	throw std::out_of_range("nytl::nth(utf8, n, size)");
}

/// \brief Converts the given utf16 string to a utf8 string.
inline std::string toUtf8(std::u16string_view utf16) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.to_bytes(utf16.begin(), utf16.end());
}

/// \brief Converts the given utf32 string to a utf8 string.
inline std::string toUtf8(std::u32string_view utf32) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.to_bytes(utf32.begin(), utf32.end());
}

/// \brief Converts the given utf8 string to a utf16 string.
inline std::u16string toUtf16(std::string_view utf8) {
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.from_bytes(utf8.begin(), utf8.end());
}

/// \brief Converts the given utf32 string to a utf16 string.
inline std::u16string toUtf16(std::u32string_view utf32) {
	return toUtf16(toUtf8(utf32));
}

/// \brief Converts the given utf8 string to a utf32 string.
inline std::u32string toUtf32(std::string_view utf8) {
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.from_bytes(utf8.begin(), utf8.end());
}

/// \brief Converts the given utf16 string to a utf32 string.
inline std::u32string toUtf32(std::u16string_view utf16) {
	return toUtf32(toUtf8(utf16));
}

} // namespace nytl

#endif // header guard
