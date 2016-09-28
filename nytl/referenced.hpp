// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Provides a base class for reference-counted types as well as a intrusvie smart pointer.

#pragma once

#ifndef NYTL_INCLUDE_REFERENCED_HPP
#define NYTL_INCLUDE_REFERENCED_HPP

#include <nytl/bits/tmpUtil.inl>

#include <atomic>
#include <utility>
#include <memory>

namespace nytl
{

///\ingroup utility
///Base class for reference counted objects.
///The reference count and its operations are atomic and therefore threadsafe.
///\tparam T The derived class as used in the CRTP idiom.
template<typename T, typename Deleter = std::default_delete<T>>
class Referenced
{
protected:
	using RefCount = std::atomic<unsigned int>;
	mutable std::tuple<RefCount, Deleter> members_ {}; //empty class optimization

protected:
	Referenced(const Deleter& deleter = {}) : members_(deleter, 0) {}
	~Referenced() = default;

public:
	///Increases the reference count.
	auto ref() const { return ++std::get<0>(members_); }

	///This function can be called from multiple threads at the same time and it will be assured
	///that only one of them will call the deleter.
	///Calling unref more times than calling ref or using the object after the last unref results
	///in undefined behaviour since the object may be deleted already.
	auto unref() const
	{
		auto cpy = std::get<0>(members_).load();

		while(true)
		{
			if(std::atomic_compare_exchange_weak(&std::get<0>(members_), &cpy, cpy - 1)) break;
			cpy = std::get<0>(members_).load(); //reload if failed
		}

		//cpy should never be 0 on correct usage.
		if(cpy == 1) std::get<1>(members_)((T*)this); //ugly but should be valid
		return cpy > 0 ? cpy - 1 : 0;
	}

	///This function can be called do decrease the reference count but not check if the this
	///was the last reference and delete the object if so.
	///Note that this function is usually faster than the plain unref call so it could be
	///preferred when the caller knows that he does not hold the last reference.
	///This function has to be used with attention.
	auto unrefNodelete() const { return --std::get<0>(members_); }

	///Returns the reference count at the moment.
	unsigned int referenceCount() const { return std::get<0>(members_).load(); }
};

///\ingroup utility
///\brief Smart pointer class for objects with built-in reference counter.
///\details If you want to make your classes reference counted types from design to be able to
///use this smart pointer, have a look at the nytl::References base class.
template<typename T> class IntrusivePtr
	: public DeriveDummy< decltype(std::declval<T>().ref()), decltype(std::declval<T>().unref())>
{
public:
	IntrusivePtr() = default;
	IntrusivePtr(T* obj) : object_(obj) { if(object_) object_->ref(); }
	IntrusivePtr(T& obj) : object_(&obj) { object_->ref(); }
	~IntrusivePtr(){ if(object_) object_->unref(); }

	IntrusivePtr(const IntrusivePtr& other) : object_(other.object_)
		{ object_->ref(); }
	IntrusivePtr& operator=(const IntrusivePtr& other)
		{ reset(other.object_); return *this; }

	IntrusivePtr(IntrusivePtr&& other) noexcept : object_(other.object_)
		{ other.object_ = nullptr; }
	IntrusivePtr& operator=(IntrusivePtr&& other) noexcept
		{ reset(); object_ = other.object_; other.object_ = nullptr; return *this; }

	void reset(T* obj = nullptr)
		{ if(obj) obj->ref(); if(object_) object_->unref(); object_ = obj;  }
	void reset(T& obj)
		{ obj.ref(); if(object_) object_->unref(); object_ = &obj;  }

	T* get() { return object_; }

	T& operator*(){ return *object_; }
	const T& operator*() const { return *object_; }

	T* operator->(){ return object_; }
	const T* operator->() const { return object_; }

	operator bool() const { return (object_ != nullptr); }
	void swap(IntrusivePtr& other) noexcept { std::swap(other.object_, object_); }

protected:
	T* object_ {nullptr};
};

}

#endif //header guard
