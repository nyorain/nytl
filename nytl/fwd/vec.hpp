// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_VEC
#define NYTL_INCLUDE_FWD_VEC

#include <cstdlib> // std::size_t
#include <cstdint> // std::uint8_t

namespace nytl {

using std::size_t;

template<size_t D, typename T> class Vec;

template<typename T> class Vec<2, T>; // nytl/vec2.hpp
template<typename T> class Vec<3, T>; // nytl/vec3.hpp

template<typename T> using Vec2 = Vec<2, T>;
template<typename T> using Vec3 = Vec<3, T>;
template<typename T> using Vec4 = Vec<4, T>;

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2ui = Vec2<unsigned int>;
using Vec2d = Vec2<double>;
using Vec2b = Vec2<bool>;
using Vec2u8 = Vec2<std::uint8_t>;
using Vec2u16 = Vec2<std::uint16_t>;
using Vec2u32 = Vec2<std::uint32_t>;
using Vec2u64 = Vec2<std::uint64_t>;
using Vec2i8 = Vec2<std::int8_t>;
using Vec2i16 = Vec2<std::int16_t>;
using Vec2i32 = Vec2<std::int32_t>;
using Vec2i64 = Vec2<std::int64_t>;

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3ui = Vec3<unsigned int>;
using Vec3d = Vec3<double>;
using Vec3c = Vec3<bool>;
using Vec3u8 = Vec3<std::uint8_t>;
using Vec3u16 = Vec3<std::uint16_t>;
using Vec3u32 = Vec3<std::uint32_t>;
using Vec3u64 = Vec3<std::uint64_t>;
using Vec3i8 = Vec3<std::int8_t>;
using Vec3i16 = Vec3<std::int16_t>;
using Vec3i32 = Vec3<std::int32_t>;
using Vec3i64 = Vec3<std::int64_t>;

using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4ui = Vec4<unsigned int>;
using Vec4d = Vec4<double>;
using Vec4b = Vec4<bool>;
using Vec4u8 = Vec4<std::uint8_t>;
using Vec4u16 = Vec4<std::uint16_t>;
using Vec4u32 = Vec4<std::uint32_t>;
using Vec4u64 = Vec4<std::uint64_t>;
using Vec4i8 = Vec4<std::int8_t>;
using Vec4i16 = Vec4<std::int16_t>;
using Vec4i32 = Vec4<std::int32_t>;
using Vec4i64 = Vec4<std::int64_t>;

}

#endif //header guard
