// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file
/// \brief Small utilities for defining/using Cloneable clases.
/// \details Small (full copyable) example for using this header:
///
/// '''cpp
/// struct Base : public nytl::AbstractCloneable {};
/// struct Derived : public nytl::DeriveCloneable<Base, Derived> { int value; };
///
/// int main()
/// {
/// 	auto derived = Derived { 42 }; // some dummy object
/// 	auto ptr = static_cast<Base*>(&derived); // a Base pointer to it loosing actual type info
///
/// 	auto copy = ptr->clone(); // we duplicate the Derived object by just having a Base pointer
/// 	auto moved = ptr->cloneMove(); // we move copy the object, i.e. object derived is now empty
///
/// 	// moved and copy are of type std::unique_ptr<Base> but hold Derived objects that are
/// 	// exact copies of the originally created 'Derived' object, i.e. .value of both objects
/// 	// has the value 42. We could do this only by having 'ptr', i.e. a pointer to an interface
/// 	// and without knowing the actual type of the object 'ptr' pointed to.
/// }
/// '''

#pragma once

#ifndef NYTL_INCLUDE_CLONE
#define NYTL_INCLUDE_CLONE

#include <memory> // std::unique_ptr

namespace nytl {

/// \brief Can be derived from to implement the cloneMove member function for 'Derived'.
/// \requires That Derived derives from this class using the CRTP idiom.
/// For this to work, Base must have defined a virtual cloneMove member function in its interface.
/// Usually used together with [nytl::CloneMovable]() or [nytl::AbstractCloneMovable]()
/// in some class Derived is derived from (i.e. Base or ancestors).
/// \module utility
template<typename Base, typename Derived>
struct DeriveCloneMovable : public Base {
	std::unique_ptr<Base> cloneMove() override;
	using Base::Base;
};

/// \brief Can be derived from to implement the clone/cloneMove member functions for 'Derived'.
/// \requires That Derived derives from this class using the CRTP idiom.
/// For this to work, Base must have defined virtual clone/cloneMove member functions in
/// its interface. Usually used together with [nytl::Cloneable]() or [nytl::AbstractCloneable]()
/// in some class Derived is derived from (i.e. Base or ancestors).
/// \module utility
template<typename Base, typename Derived>
struct DeriveCloneable : public DeriveCloneMovable<Base, Derived> {
	std::unique_ptr<Base> clone() const override;
	using Base::Base;
};

/// \brief Can be derived from to make cloneMoving for interfaces possible.
/// If some interface class derived from this type using the CRTP idiom (i.e. 'T' is the
/// deriving class), objects of this interface will be able to be cloneMoved, i.e. they can
/// be move-copied into a new object without knowing their exact implementation type.
/// Implementing classes usually use DeriveCloneMovable to implement it.
/// To additionally enable normal cloning (i.e. copying without moving), see
/// [nytl::AbstractCloneable]().
/// \module utility
template<typename T>
struct AbstractCloneMovable {
	virtual std::unique_ptr<T> cloneMove() = 0;
	virtual ~AbstractCloneMovable() = default;
};

/// \brief Can be derived from to make cloning for interfaces possible.
/// If some interface class derived from this type using the CRTP idiom (i.e. 'T' is the
/// deriving class), objects of this interface will be able to be cloned, i.e. they can
/// be duplicated to a new object without knowing their exact implementation type.
/// Implementing classes usually use DeriveCloneable to implement it.
/// To only enable cloneMoving (i.e. don't require implementations to actually duplicate objects),
/// see [nytl::AbstractCloneMovable]().
/// \module utility
template<typename T>
struct AbstractCloneable : public AbstractCloneMovable<T> {
	virtual std::unique_ptr<T> clone() const = 0;
};

/// \brief Can be used to add the clone interface to a class as well as already implementing it.
/// \module utility
template<typename T> using Cloneable = DeriveCloneable<AbstractCloneable<T>, T>;

/// \brief Can be used to add the cloneMove interface to a class as well as already implementing it.
/// \module utility
template<typename T> using CloneMovable = DeriveCloneMovable<AbstractCloneMovable<T>, T>;

template<typename Base, typename Derived>
std::unique_ptr<Base> DeriveCloneMovable<Base, Derived>::cloneMove()
	{ return std::make_unique<Derived>(std::move(static_cast<Derived&>(*this))); }

template<typename Base, typename Derived>
std::unique_ptr<Base> DeriveCloneable<Base, Derived>::clone() const
	{ return std::make_unique<Derived>(static_cast<Derived&>(*this)); }


}

#endif //header guard
