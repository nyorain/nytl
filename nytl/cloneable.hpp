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
///\brief Small utilities for defining/using cloneable clases.

#pragma once

#include <nytl/make_unique.hpp>
#include <memory>
#include <vector>
#include <type_traits>

namespace nytl
{

///Utility template to derive from a class with a virtual clone function.
template<typename Base, typename Derived> 
class deriveCloneable : public Base
{
public:
    virtual std::unique_ptr<Base> clone() const override
		{ return make_unique<Derived>(*(static_cast<const Derived*>(this))); }
};


///\brief Utility function to copy a vector of cloneable objects by cloning.
///\details This can be useful if one has a vector of polymorph objects which
///can not be copy constructed (e.g. vector<SomeAbstractBaseClass*>), especially
///when dealing with smart pointers like std::unique_ptr.
///\param vectorObject A vector of cloneable objects (objects with a clone() member function).
///\return A std::vector of cloned objects.
template<class A> auto cloneVector(const std::vector<A>& vectorObject)
	-> std::vector<typename std::result_of<decltype(vectorObject[0]->clone())>::type>
{
    std::vector<A> ret;
    ret.reserve(vectorObject.size());

    for(auto& val : vectorObject)
    {
		//? what is this implementation?
        auto&& cpy = val->clone();
        auto& cpy2 = (A&) cpy;
        ret.emplace_back(std::move(cpy2));
    }
    return ret;
}

}
