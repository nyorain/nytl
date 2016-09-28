// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines the helper base classes NonCopyable and NonMovable to derive from.

#pragma once

#ifndef NYTL_INCLUDE_NONCOPYABLE_HPP
#define NYTL_INCLUDE_NONCOPYABLE_HPP

namespace nytl
{

//\ingroup utility
///Derive from this class to make it impossible to copy objects of the derived class.
class NonCopyable
{
private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator =(const NonCopyable&) = delete;

protected:
	NonCopyable() noexcept = default;

	NonCopyable(NonCopyable&&) noexcept = default;
	NonCopyable& operator=(NonCopyable&&) noexcept = default;
};

//\ingroup utility
///Derive from this class to make it impossible to copy or move objects of the derived class.
class NonMovable
{
private:
	NonMovable(const NonMovable&) = delete;
	NonMovable& operator =(const NonMovable&) = delete;
	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;

protected:
	NonMovable() noexcept = default;
};

}

#endif //header guard
