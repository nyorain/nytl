// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Small utilities for defining/using Cloneable clases.

#pragma once

#ifndef NYTL_INCLUDE_CLONE_HPP
#define NYTL_INCLUDE_CLONE_HPP

#include <memory>
#include <vector>
#include <type_traits>

namespace nytl
{

///\brief CloneMoveable base class
///\ingroup utilty
template<typename T>
class CloneMovable
{
private:
	virtual T* cloneMove() { return new T(std::move(static_cast<T&>(*this))); }
	template<typename X> friend std::unique_ptr<X> cloneMove(X&);

protected:
	virtual ~CloneMovable() = default;
};

///\brief Cloneable base class
///\ingroup utility
template<typename T>
class Cloneable : public CloneMovable<T>
{
	virtual T* clone() const { return new T(static_cast<const T&>(*this)); }
	template<typename X> friend std::unique_ptr<X> clone(const X&);
};

///\brief Abstract CloneMovable base class
///\ingroup utility
template<typename T>
class AbstractCloneMovable
{
private:
	template<typename X> friend std::unique_ptr<X> cloneMove(X&);
	virtual T* cloneMove() = 0;

protected:
	virtual ~AbstractCloneMovable() = default;
};

///\brief Abstract Cloneable base class
///\ingroup utility
template<typename T>
class AbstractCloneable : public AbstractCloneMovable<T>
{
	template<typename X> friend std::unique_ptr<X> clone(const X&);
	virtual T* clone() const = 0;
};

///\brief Utility template to derive from a class with a virtual cloneMove function.
///\ingroup utility
template<typename Base, typename Derived>
class DeriveCloneMovable : public Base
{
private:
	template<typename X> friend std::unique_ptr<X> cloneMove(X&);
    virtual Base* cloneMove() override
		{ return new Derived(std::move(static_cast<Derived&>(*this))); }

protected:
	using CloneMovableBase = DeriveCloneMovable;
	using Base::Base;
};


///\brief Utility template to derive from a class with a virtual clone function.
///\ingroup utility
template<typename Base, typename Derived>
class DeriveCloneable : public DeriveCloneMovable<Base, Derived>
{
private:
	template<typename X> friend std::unique_ptr<X> clone(const X&);
    virtual Base* clone() const override //Base return type since it uses CRTP
		{ return new Derived(static_cast<const Derived&>(*this)); }

protected:
	using CloneableBase = DeriveCloneable;
	using Base::Base;
};

///\ingroup utility
///\{
///\brief Clones the given (Cloneable) object in a unique_ptr.
///\details This function should always be called instead of obj.clone() since it is
///able to return a unique_ptr while still being able to have convarient return types
///in the member clone function. See Cloneable, AbstractCloneable and derviveCloneable
///for more inforMation.
template<typename T>
std::unique_ptr<T> clone(const T& value)
{
	return std::unique_ptr<T>(static_cast<T*>(value.clone()));
}

///\ingroup utility
template<typename T>
std::unique_ptr<T> clone(const T* value)
{
	return value ? clone(*value) : nullptr;
}

//XXX: good idea?
///\ingroup utility
template<typename T>
std::unique_ptr<T> clone(const std::unique_ptr<T>& value)
{
	return value ? clone(*value) : nullptr;
}
///\}

///\ingroup utility
///Clones the object by moving it. The given argument will no longer be valid after this call.
template<typename T>
std::unique_ptr<T> cloneMove(T& value)
{
	return std::unique_ptr<T>(static_cast<T*>(value.cloneMove()));
}

///\ingroup utility
///\brief Utility function to copy a Vector of Cloneable objects by cloning.
///\details This can be useful if one has a Vector of polymorph objects which
///can not be copy constructed (e.g. Vector<SomeAbstractBaseClass*>), especially
///when dealing with smart pointers like std::unique_ptr.
///\param VectorObject A Vector of Cloneable objects (objects with a clone() member function).
///\return A std::vector of cloned objects.
template<class A> std::vector<decltype(clone(A{}))>
cloneVector(const std::vector<A>& VectorObject)
{
    std::vector<A> ret;
    ret.reserve(VectorObject.size());

    for(auto& val : VectorObject)
        ret.emplace_back(clone(val));

    return ret;
}

}

#endif //header guard
