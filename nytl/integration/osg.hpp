// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

//XXX NOTE: include all osg matrix and vector files before including this file.

#pragma once

#ifndef NYTL_INCLUDE_INTEGRATION_OSG_HPP
#define NYTL_INCLUDE_INTEGRATION_OSG_HPP

#include <nytl/convert.hpp>
#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

namespace nytl
{

Mat4f fromOsg(const osg::Matrixf& m) { return reinterpret_cast<const Mat4f&>(m); }
Mat4d fromOsg(const osg::Matrixd& m) { return reinterpret_cast<const Mat4d&>(m); }

Vec2f fromOsg(const osg::Vec2f& v) { return reinterpret_cast<const Vec2f&>(v); }
Vec3f fromOsg(const osg::Vec3f& v) { return reinterpret_cast<const Vec3f&>(v); }
Vec4f fromOsg(const osg::Vec4f& v) { return reinterpret_cast<const Vec4f&>(v); }

Vec2d fromOsg(const osg::Vec2d& v) { return reinterpret_cast<const Vec2d&>(v); }
Vec3d fromOsg(const osg::Vec3d& v) { return reinterpret_cast<const Vec3d&>(v); }
Vec4d fromOsg(const osg::Vec4d& v) { return reinterpret_cast<const Vec4d&>(v); }

Vec2i fromOsg(const osg::Vec2i& v) { return reinterpret_cast<const Vec2i&>(v); }
Vec3i fromOsg(const osg::Vec3i& v) { return reinterpret_cast<const Vec3i&>(v); }
Vec4i fromOsg(const osg::Vec4i& v) { return reinterpret_cast<const Vec4i&>(v); }

Vec2ui fromOsg(const osg::Vec2ui& v) { return reinterpret_cast<const Vec2ui&>(v); }
Vec3ui fromOsg(const osg::Vec3ui& v) { return reinterpret_cast<const Vec3ui&>(v); }
Vec4ui fromOsg(const osg::Vec4ui& v) { return reinterpret_cast<const Vec4ui&>(v); }

Vec2c fromOsg(const osg::Vec2b& v) { return reinterpret_cast<const Vec2c&>(v); }
Vec3c fromOsg(const osg::Vec3b& v) { return reinterpret_cast<const Vec3c&>(v); }
Vec4c fromOsg(const osg::Vec4b& v) { return reinterpret_cast<const Vec4c&>(v); }

Vec2uc fromOsg(const osg::Vec2ub& v) { return reinterpret_cast<const Vec2uc&>(v); }
Vec3uc fromOsg(const osg::Vec3ub& v) { return reinterpret_cast<const Vec3uc&>(v); }
Vec4uc fromOsg(const osg::Vec4ub& v) { return reinterpret_cast<const Vec4uc&>(v); }

Vec2s fromOsg(const osg::Vec2s& v) { return reinterpret_cast<const Vec2s&>(v); }
Vec3s fromOsg(const osg::Vec3s& v) { return reinterpret_cast<const Vec3s&>(v); }
Vec4s fromOsg(const osg::Vec4s& v) { return reinterpret_cast<const Vec4s&>(v); }

Vec2us fromOsg(const osg::Vec2us& v) { return reinterpret_cast<const Vec2us&>(v); }
Vec3us fromOsg(const osg::Vec3us& v) { return reinterpret_cast<const Vec3us&>(v); }
Vec4us fromOsg(const osg::Vec4us& v) { return reinterpret_cast<const Vec4us&>(v); }

//to
osg::Matrixf toOsg(const Mat4f& m) { return reinterpret_cast<const osg::Matrixf&>(m); }
osg::Matrixd toOsg(const Mat4d& m) { return reinterpret_cast<const osg::Matrixd&>(m); }

osg::Vec2f toOsg(const Vec2f& v) { return reinterpret_cast<const osg::Vec2f&>(v); }
osg::Vec3f toOsg(const Vec3f& v) { return reinterpret_cast<const osg::Vec3f&>(v); }
osg::Vec4f toOsg(const Vec4f& v) { return reinterpret_cast<const osg::Vec4f&>(v); }

osg::Vec2d toOsg(const Vec2d& v) { return reinterpret_cast<const osg::Vec2d&>(v); }
osg::Vec3d toOsg(const Vec3d& v) { return reinterpret_cast<const osg::Vec3d&>(v); }
osg::Vec4d toOsg(const Vec4d& v) { return reinterpret_cast<const osg::Vec4d&>(v); }

osg::Vec2i toOsg(const Vec2i& v) { return reinterpret_cast<const osg::Vec2i&>(v); }
osg::Vec3i toOsg(const Vec3i& v) { return reinterpret_cast<const osg::Vec3i&>(v); }
osg::Vec4i toOsg(const Vec4i& v) { return reinterpret_cast<const osg::Vec4i&>(v); }

osg::Vec2ui toOsg(const Vec2ui& v) { return reinterpret_cast<const osg::Vec2ui&>(v); }
osg::Vec3ui toOsg(const Vec3ui& v) { return reinterpret_cast<const osg::Vec3ui&>(v); }
osg::Vec4ui toOsg(const Vec4ui& v) { return reinterpret_cast<const osg::Vec4ui&>(v); }

osg::Vec2b toOsg(const Vec2c& v) { return reinterpret_cast<const osg::Vec2b&>(v); }
osg::Vec3b toOsg(const Vec3c& v) { return reinterpret_cast<const osg::Vec3b&>(v); }
osg::Vec4b toOsg(const Vec4c& v) { return reinterpret_cast<const osg::Vec4b&>(v); }

osg::Vec2ub toOsg(const Vec2uc& v) { return reinterpret_cast<const osg::Vec2ub&>(v); }
osg::Vec3ub toOsg(const Vec3uc& v) { return reinterpret_cast<const osg::Vec3ub&>(v); }
osg::Vec4ub toOsg(const Vec4uc& v) { return reinterpret_cast<const osg::Vec4ub&>(v); }

osg::Vec2s toOsg(const Vec2s& v) { return reinterpret_cast<const osg::Vec2s&>(v); }
osg::Vec3s toOsg(const Vec3s& v) { return reinterpret_cast<const osg::Vec3s&>(v); }
osg::Vec4s toOsg(const Vec4s& v) { return reinterpret_cast<const osg::Vec4s&>(v); }

osg::Vec2us toOsg(const Vec2us& v) { return reinterpret_cast<const osg::Vec2us&>(v); }
osg::Vec3us toOsg(const Vec3us& v) { return reinterpret_cast<const osg::Vec3us&>(v); }
osg::Vec4us toOsg(const Vec4us& v) { return reinterpret_cast<const osg::Vec4us&>(v); }

//Converter convinience specializations for all to/from glm conversions
template<typename T, typename O>
struct Converter<T, O,
	std::enable_if_t<std::is_convertible<decltype(fromOsg(std::declval<T>())), O>::value>>
{
	static O call(const T& lhs) { return fromOsg(lhs); }
};

template<typename T, typename O>
struct Converter<T, O,
	std::enable_if_t<std::is_convertible<decltype(toOsg(std::declval<T>())), O>::value>>
{
	static O call(const T& lhs) { return toOsg(lhs); }
};

}

#endif //header guard

// The classes that have to be defined.
// namespace osg
// {
//
// class Matrixf;
// class Matrixd;
//
// class Vec2f;
// class Vec3f;
// class Vec4f;
//
// class Vec2d;
// class Vec3d;
// class Vec4d;
//
// class Vec2i;
// class Vec3i;
// class Vec4i;
//
// class Vec2ui;
// class Vec3ui;
// class Vec4ui;
//
// class Vec2b;
// class Vec3b;
// class Vec4b;
//
// class Vec2ub;
// class Vec3ub;
// class Vec4ub;
//
// class Vec2s;
// class Vec3s;
// class Vec4s;
//
// class Vec2us;
// class Vec3us;
// class Vec4us;
//
// }

// The osg headers that one has to include
// #include <osg/Matrixf>
// #include <osg/Matrixd>
//
// #include <osg/Vec2f>
// #include <osg/Vec2d>
// #include <osg/Vec2i>
// #include <osg/Vec2ui>
// #include <osg/Vec2b>
// #include <osg/Vec2ub>
// #include <osg/Vec2s>
// #include <osg/Vec2us>
//
// #include <osg/Vec3f>
// #include <osg/Vec3d>
// #include <osg/Vec3i>
// #include <osg/Vec3ui>
// #include <osg/Vec3b>
// #include <osg/Vec3ub>
// #include <osg/Vec3s>
// #include <osg/Vec3us>
//
// #include <osg/Vec4f>
// #include <osg/Vec4d>
// #include <osg/Vec4i>
// #include <osg/Vec4ui>
// #include <osg/Vec4b>
// #include <osg/Vec4ub>
// #include <osg/Vec4s>
// #include <osg/Vec4us>
