// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_REFERENCEITERATION_INL
#define NYTL_INCLUDE_REFERENCEITERATION_INL

#include <type_traits>
#include <iterator>

namespace nytl
{

///\ingroup utitliy
///Transforms the T iterator over pointers into a reference-iterator.
template<typename T>
class ReferenceIterator : public T
{
public:
	ReferenceIterator(const T& other) : T(other) {}

	auto operator->() -> decltype(*T{}) { return *(T::operator->()); }
	auto operator*() -> typename std::remove_pointer<decltype(*T{})>::type
		{ return *(T::operator*()); }
};

///\ingroup utitliy
///\brief Automatically transforms a pointer iterator into a ReferenceIterator.
template<typename T> ReferenceIterator<T>
makeReferenceIterator(const T& it)
{
	return ReferenceIterator<T>(it);
}

///\ingroup utitliy
///Allows reference iteration (like range-based for loop) over an object.
template<typename T>
class ReferenceIteration
{
public:
	T* object_;

	using iterator = ReferenceIterator<decltype(object_->begin())>;
	using const_iterator = ReferenceIterator<decltype(object_->cbegin())>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	ReferenceIteration(T& object) : object_(&object) {}

	iterator begin(){ object_->begin(); }
	const_iterator begin() const { return object_->begin(); }
	const_iterator cbegin() const { return object_->cbegin(); }
	reverse_iterator rbegin() { return object_->rbegin(); }
	const_reverse_iterator rbegin() const { return object_->rbegin(); }
	const_reverse_iterator crbegin() const { return object_->crbegin(); }

	iterator end(){ object_->end(); }
	const_iterator end() const { return object_->end(); }
	const_iterator cend() const { return object_->cend(); }
	reverse_iterator rend(){ return object_->rend(); }
	const_reverse_iterator rend() const { return object_->rend(); }
	const_reverse_iterator crend() const { return object_->crend(); }
};

///\ingroup utitliy
template<typename T> ReferenceIterator<T>
makeReferenceIteration(const T& obj)
{
	return ReferenceIteration<T>(obj);
}

}

#endif //header guard
