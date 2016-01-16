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

#include <memory>
#include <vector>
#include <type_traits>

namespace nytl
{

///\brief Cloneable base class
template<typename T>
class cloneable
{
private:
	virtual T* clone() const { return new T(static_cast<const T&>(*this)); }

	template<typename X> 
	friend std::unique_ptr<X> clone(const X&);

protected:
	virtual ~cloneable() = default;
};

///\brief Abstract cloneable base class
template<typename T>
class abstractCloneable
{
private:
	virtual T* clone() const = 0;

	template<typename X> 
	friend std::unique_ptr<X> clone(const X&);

protected:
	virtual ~abstractCloneable() = default;
};

///\brief Utility template to derive from a class with a virtual clone function.
///\ingroup utility
template<typename Base, typename Derived> 
class deriveCloneable : public Base
{
private:
    virtual Base* clone() const override
		{ return new Derived(*(static_cast<const Derived*>(this))); }

	template<typename X> 
	friend std::unique_ptr<X> clone(const X&);

public:
	using Base::Base;
};

///\brief Clones the given (cloneable) object in a unique_ptr.
///\details This function should always be called instead of obj.clone() since it is
///able to return a unique_ptr while still being able to have convarient return types
///in the member clone function. See cloneable, abstractCloneable and derviveCloneable
///for more information.
template<typename T>
std::unique_ptr<T> clone(const T& value)
{
	return std::unique_ptr<T>(static_cast<T*>(value.clone()));
}

template<typename T>
std::unique_ptr<T> clone(const T* value)
{
	return clone(*value);
}

//XXX: good idea?
template<typename T>
std::unique_ptr<T> clone(const std::unique_ptr<T>& value)
{
	return clone(*value);
}

///\brief Utility function to copy a vector of cloneable objects by cloning.
///\details This can be useful if one has a vector of polymorph objects which
///can not be copy constructed (e.g. vector<SomeAbstractBaseClass*>), especially
///when dealing with smart pointers like std::unique_ptr.
///\param vectorObject A vector of cloneable objects (objects with a clone() member function).
///\return A std::vector of cloned objects.
template<class A> std::vector<decltype(clone(A{}))> 
cloneVector(const std::vector<A>& vectorObject)
{
    std::vector<A> ret;
    ret.reserve(vectorObject.size());

    for(auto& val : vectorObject)
        ret.emplace_back(clone(val));

    return ret;
}

}
