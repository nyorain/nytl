// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_CLONE
#define NYTL_INCLUDE_CLONE

#include <memory> // std::unique_ptr

namespace nytl {

/// \brief Can be used to clone a Cloneable object.
/// Will perform a copy of the actual type the interface reference references.
/// \requires Type 'T' shall be Cloneable (i.e. derived from a nytl::Cloneable or
/// nytl::AbstractCloneable class using nytl::DeriveCloneable).
/// \module utility
template<typename T>
std::unique_ptr<T> clone(const T& obj) {
	return std::unique_ptr<T>(static_cast<T*>(obj.doClone()));
}

/// \brief Can be used to cloneMove a CloneMovable object.
/// Will move from the given object and return a moved copy of it preserving the actual
/// implementation type the interface reference references.
/// \requires Type 'T' shall be derived from a nytl::CloneMovable or
/// nytl::AbstractCloneMovable class using nytl::DeriveCloneMovable.
/// \module utility
template<typename T>
std::unique_ptr<T> cloneMove(T& obj) {
	return std::unique_ptr<T>(static_cast<T*>(obj.doCloneMove()));
}

/// \brief Can be derived from to implement the cloneMove member function for 'Derived'.
/// \requires Type 'Derived' shall derive from this class using the CRTP idiom.
/// For this to work, Base must have defined a virtual cloneMove member function in its interface.
/// Usually used together with [nytl::CloneMovable]() or [nytl::AbstractCloneMovable]()
/// in some class Derived is derived from (i.e. Base or ancestors).
/// \module utility
template<typename Derived, typename... Bases>
class DeriveCloneMovable : public Bases... {
protected:
	void* doCloneMove() override; // Base return type since CRTP
	template<typename O> friend std::unique_ptr<O> cloneMove(O&);
};

template<typename Derived, typename Base>
class DeriveCloneMovable<Derived, Base> : public Base {
protected:
	using Base::Base;
	void* doCloneMove() override; // Base return type since CRTP
	template<typename O> friend std::unique_ptr<O> cloneMove(O&);
};

/// \brief Can be derived from to implement the clone/cloneMove member functions for 'Derived'.
/// \requires Type 'Derived' shall derive from this class using the CRTP idiom.
/// For this to work, Base must have defined virtual clone/cloneMove member functions in
/// its interface. Usually used together with [nytl::Cloneable]() or [nytl::AbstractCloneable]()
/// in some class Derived is derived from (i.e. Base or ancestors).
/// \module utility
template<typename Derived, typename... Bases>
class DeriveCloneable : public DeriveCloneMovable<Derived, Bases...> {
protected:
	using DeriveCloneMovable<Derived, Bases...>::DeriveCloneMovable;
	void* doClone() const override; // Base return type since CRTP
	template<typename O> friend std::unique_ptr<O> clone(const O&);
};

/// \brief Can be derived from to make clone-moving for interfaces possible.
/// If some interface class derived from this type using the CRTP idiom (i.e. 'T' is the
/// deriving class), objects of this interface will be able to be clone-moved, i.e. they can
/// be move-copied into a new object without knowing their exact implementation type.
/// Implementing classes usually use DeriveCloneMovable to implement it.
/// To additionally enable normal cloning (i.e. copying without moving), see
/// [nytl::AbstractCloneable]().
/// \module utility
template<typename T>
class AbstractCloneMovable {
protected:
	virtual void* doCloneMove() = 0;
	virtual ~AbstractCloneMovable() = default;
	template<typename O> friend std::unique_ptr<O> cloneMove(O&);
};

/// \brief Can be derived from to make cloning for abstract classes possible.
/// If some interface class derived from this type using the CRTP idiom (i.e. 'T' is the
/// deriving class), objects of this interface will be able to be cloned, i.e. they can
/// be duplicated to a new object without knowing their exact implementation type.
/// Implementing classes usually use DeriveCloneable to implement it.
/// To only enable clone-moving (i.e. don't require implementations to actually duplicate
/// objects), see [nytl::AbstractCloneMovable]().
/// \module utility
template<typename T>
class AbstractCloneable : public AbstractCloneMovable<T> {
protected:
	virtual void* doClone() const = 0;
	template<typename O> friend std::unique_ptr<O> clone(const O&);
};


/// \brief Can be used to add the clone-move interface to a class as well as already implement it.
/// \module utility
template<typename T>
class CloneMovable : public AbstractCloneMovable<T> {
protected:
	void* doCloneMove() override { return new T(std::move(static_cast<T&>(*this))); }
	template<typename O> friend std::unique_ptr<O> cloneMove(O&);
};

/// \brief Can be used to add the clone interface to a class as well as already implement it.
/// \module utility
template<typename T>
class Cloneable : public AbstractCloneable<T> {
protected:
	void* doCloneMove() override { return new T(std::move(static_cast<T&>(*this))); }
	void* doClone() const override { return new T(static_cast<const T&>(*this)); }
	template<typename O> friend std::unique_ptr<O> clone(const O&);
	template<typename O> friend std::unique_ptr<O> cloneMove(const O&);
};

// - derive class implementation -
template<typename Derived, typename... Bases>
void* DeriveCloneMovable<Derived, Bases...>::doCloneMove()
	{ return new Derived(std::move(static_cast<Derived&>(*this))); }

template<typename Derived, typename Base>
void* DeriveCloneMovable<Derived, Base>::doCloneMove()
	{ return new Derived(std::move(static_cast<Derived&>(*this))); }

template<typename Derived, typename... Bases>
void* DeriveCloneable<Derived, Bases...>::doClone() const
	{ return new Derived(static_cast<const Derived&>(*this)); }

}

#endif //header guard

/// \file
/// \brief Small utilities for defining/using Cloneable classes.
/// \details Small (full copyable) example for using this header:
///
/// ```cpp
/// // We have to use nytl::AbstractCloneable sine Base is already abstract.
/// // Otherwise, just use nytl::Cloneable<T>. If you only want to nytl::cloneMove
/// // and don't care about nytl::clone, use AbstractCloneMovable/CloneMovable
/// // and the associated derivation helpers.
/// struct Base : public nytl::AbstractCloneable<Base> {
/// 	virtual int value() const = 0;
/// };
///
/// struct Derived : public nytl::DeriveCloneable<Derived, Base> {
/// 	int value_;
/// 	int value() const override { return value_; }
/// };
///
/// int main() / {
/// 	auto derived = Derived {}; // some dummy object with value 42
///		derived.value_ = 42;
///
/// 	auto ptr = static_cast<Base*>(&derived); // a Base pointer to it loosing actual type info
///
/// 	auto copy = nytl::clone(*ptr); // we duplicate the Derived object by the base ptr
/// 	auto moved = nytl::cloneMove(*ptr); // we move from ptr (-> derived) into moved
///
/// 	// copy and moved are both objects of type Derived (wrapped in a unique_ptr) that were
/// 	// copied from derived, so both have the value 42. We could copy this value
/// 	// by just having a Base pointer.
///		std::cout << copy->value() << "\n"; // will output 42
///		std::cout << moved->value() << "\n"; // will output 42
/// }
/// ```
///
/// Instead of `Base` you could also pass multiple bases from which to derive.
/// If you only derive from one type you can still use its constructor, it is
/// also used by DeriveCloneable/DeriveCloneMovable.

// implementation notes: The rather complex doClone/doCloneMove virtual implementation
// is needed since one cannot return covariant return types from virtual functions when using CRTP.
// we have to use void* since we allow multiple base classes.
