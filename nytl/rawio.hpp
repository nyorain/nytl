// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_RAW_IO
#define NYTL_INCLUDE_RAW_IO

/// Contains small wrappers around memcpy and/or reinterpret cast that
/// provide a safer and easier-to-use way to perform raw, byte-buffer
/// base io.

#include <cstddef>
#include <type_traits>
#include <cstring>

namespace nytl {

/// Writes the given value into the given raw buffer pointer.
/// Returns a reference to the written value (inside the buffer).
template<typename T>
auto& write(std::byte*& buf, T&& obj) {
	using TT = std::remove_cv_t<std::remove_reference_t<T>>;
	std::memcpy(buf, &obj, sizeof(obj));
	auto& ret = reinterpret_cast<TT&>(buf);
	buf += sizeof(obj);
	return ret;
}

template<typename T>
auto& write(std::byte*&& buf, T&& obj) {
	using TT = std::remove_cv_t<std::remove_reference_t<T>>;
	std::memcpy(buf, &obj, sizeof(obj));
	return reinterpret_cast<TT&>(buf);
}

/// Reads a value of the specified type from the given buffer pointer.
template<typename T>
const T& read(const std::byte*& buf) {
	auto& ret = reinterpret_cast<const T&>(buf);
	buf += sizeof(T);
	return ret;
}

template<typename T>
T& read(std::byte*& buf) {
	auto& ret = reinterpret_cast<T&>(buf);
	buf += sizeof(T);
	return ret;
}

template<typename T>
const T& read(const std::byte*&& buf) {
	return reinterpret_cast<const T&>(buf);
}

template<typename T>
T& read(std::byte*&& buf) {
	return reinterpret_cast<T&>(buf);
}

} // namespace nytl

#endif // header guard
