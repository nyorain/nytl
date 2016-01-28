/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
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
///\brief Defines the helper base classes nonCopyable and nonMoveable to derive from.

#pragma once

namespace nytl
{

//\ingroup utility
///Derive from this class to make it impossible to copy objects of the derived class.
class nonCopyable
{
private:
	nonCopyable(const nonCopyable&) = delete;
	nonCopyable& operator =(const nonCopyable&) = delete;
protected:
	nonCopyable() noexcept = default;
	nonCopyable(nonCopyable&) noexcept = default;

	nonCopyable(nonCopyable&&) noexcept = default;
	nonCopyable& operator=(nonCopyable&&) noexcept = default;
};

//\ingroup utility
///Derive from this class to make it impossible to copy or move objects of the derived class.
class nonMoveable
{
private:
	nonMoveable(const nonMoveable&) = delete;
	nonMoveable& operator =(const nonMoveable&) = delete;
	nonMoveable(nonMoveable&&) = delete;
	nonMoveable& operator=(nonMoveable&&) = delete;

protected:
	nonMoveable() noexcept = default;
	nonMoveable(nonMoveable&) noexcept = default;
};

}

