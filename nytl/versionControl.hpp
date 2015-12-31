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
///\brief Provides a utility class to handle different versions of a function.

//TODO: files name is missleading, version control is usually associated with sth like
//git or sourceforge.

#pragma once

#include <unordered_map>
#include <functional>
#include <cstdint>

namespace nytl
{

template<typename, typename = std::uint32_t>
class versionHandler;

template<typename Ret, typename ... Args, typename Version>
class versionHandler<Ret(Args...), Version>
{
protected:
	std::unordered_map<Version, std::function<Ret(Args...)>> versions_;

public:
	void addVersion(Version version, std::function<Ret(Args...)> func){ versions_[version] = func; };
	bool hasVersion(Version version) const { return versions_.find(version) != versions_.cend(); }

	Ret call(Version version, Args ... args){ return versions_[version](args ...); }
	Ret operator()(Version version, Args ... args){ return call(version, args ...); }
};

}
