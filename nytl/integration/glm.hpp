// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

//XXX NOTE: include glm.hpp before including this file

//nytl make not guarantees for glm integration since glm vectors use non-standeard C++
//union access. Although it works on all curruent compilers, it may lead to issues in the
//future (unlikely).

#pragma once

#ifndef NYTL_INCLUDE_INTEGRATION_GLM_HPP
#define NYTL_INCLUDE_INTEGRATION_GLM_HPP

#include <nytl/convert.hpp>
#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

namespace nytl
{

template<typename T> using Raw = std::remove_reference_t<std::remove_cv_t<T>>;

//vec
template<glm::precision P = glm::defaultp, typename T>
glm::tvec4<Raw<T>, P> toGlm(const nytl::Vec4<T>& vec)
	{ return {vec.x, vec.y, vec.z, vec.w}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tvec3<Raw<T>, P> toGlm(const nytl::Vec3<T>& vec)
	{ return {vec.x, vec.y, vec.z}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tvec2<Raw<T>, P> toGlm(const nytl::Vec2<T>& vec)
	{ return {vec.x, vec.y}; }



template<glm::precision P, typename T>
nytl::Vec4<T> fromGlm(const glm::tvec4<T, P>& vec)
	{ return {vec.x, vec.y, vec.z, vec.w}; }

template<glm::precision P, typename T>
nytl::Vec3<T> fromGlm(const glm::tvec3<T, P>& vec)
	{ return {vec.x, vec.y, vec.z}; }

template<glm::precision P, typename T>
nytl::Vec2<T> fromGlm(const glm::tvec2<T, P>& vec)
	{ return {vec.x, vec.y}; }

//mat
template<glm::precision P = glm::defaultp, typename T>
glm::tmat2x2<T, P> toGlm(const nytl::Mat2<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat2x3<T, P> toGlm(const nytl::Mat32<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat2x4<T, P> toGlm(const nytl::Mat42<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1))}; }


template<glm::precision P = glm::defaultp, typename T>
glm::tmat3x2<T, P> toGlm(const nytl::Mat23<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat3x3<T, P> toGlm(const nytl::Mat3<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat3x4<T, P> toGlm(const nytl::Mat43<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2))}; }


template<glm::precision P = glm::defaultp, typename T>
glm::tmat4x2<T, P> toGlm(const nytl::Mat24<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2)), toGlm(m.col(3))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat4x3<T, P> toGlm(const nytl::Mat34<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2)), toGlm(m.col(3))}; }

template<glm::precision P = glm::defaultp, typename T>
glm::tmat4x4<T, P> toGlm(const nytl::Mat4<T>& m)
	{ return {toGlm(m.col(0)), toGlm(m.col(1)), toGlm(m.col(2)), toGlm(m.col(3))}; }



template<glm::precision P = glm::defaultp, typename T>
nytl::Mat2<T> fromGlm(const glm::tmat2x2<T, P>& m)
	{ return transpose(Mat2<T>{fromGlm(m[0]), fromGlm(m[1])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat32<T> fromGlm(const glm::tmat2x3<T, P>& m)
	{ return transpose(Mat23<T>{fromGlm(m[0]), fromGlm(m[1])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat42<T> fromGlm(const glm::tmat2x4<T, P>& m)
	{ return transpose(Mat24<T>{fromGlm(m[0]), fromGlm(m[1])}); }


template<glm::precision P = glm::defaultp, typename T>
nytl::Mat23<T> fromGlm(const glm::tmat3x2<T, P>& m)
	{ return transpose(Mat32<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat3<T> fromGlm(const glm::tmat3x3<T, P>& m)
	{ return transpose(Mat3<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat43<T> fromGlm(const glm::tmat3x4<T, P>& m)
	{ return transpose(Mat34<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2])}); }


template<glm::precision P = glm::defaultp, typename T>
nytl::Mat24<T> fromGlm(const glm::tmat4x2<T, P>& m)
	{ return transpose(Mat42<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2]), fromGlm(m[3])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat34<T> fromGlm(const glm::tmat4x3<T, P>& m)
	{ return transpose(Mat43<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2]), fromGlm(m[3])}); }

template<glm::precision P = glm::defaultp, typename T>
nytl::Mat4<T> fromGlm(const glm::tmat4x4<T, P>& m)
	{ return transpose(Mat4<T>{fromGlm(m[0]), fromGlm(m[1]), fromGlm(m[2]), fromGlm(m[3])}); }


//Converter convinience specializations for all to/from glm conversions
template<typename T, typename O>
struct Converter<T, O,
	std::enable_if_t<std::is_convertible<decltype(fromGlm(std::declval<T>())), O>::value>>
{
	static O call(const T& lhs) { return fromGlm(lhs); }
};

template<typename T, typename O>
struct Converter<T, O,
	std::enable_if_t<std::is_convertible<decltype(toGlm(std::declval<T>())), O>::value>>
{
	static O call(const T& lhs) { return toGlm(lhs); }
};

}

#endif //header guard
