// The MIT License (MIT)
//
// Copyright (c) 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

///\file
///\brief Provides a base class for reference-counted types as well as a intrusvie smart pointer.

#pragma once

#ifndef NYTL_INCLUDE_REFERENCED_HPP
#define NYTL_INCLUDE_REFERENCED_HPP

#include <atomic>
#include <utility>
#include <memory>

namespace nytl
{

//TODO: const correctness (see unref T* cast)
//virtual desctructor?

///\ingroup utility
///Base class for reference counted objects.
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
	auto ref() const { return ++std::get<0>(members_); }

	///This function can be called from multiple threads at the same time and it will be assured
	///that only one of them will call the deleter.
	auto unref() const
	{
		auto cpy = std::get<0>(members_).load();

		while(true)
		{
			if(std::atomic_compare_exchange_weak(&std::get<0>(members_), &cpy, cpy - 1)) break;
			cpy = std::get<0>(members_).load(); //reload if failed
		}

		//cpy should never be 0 on correct usage.
		if(cpy <= 1) std::get<1>(members_)((T*)this); ///XXX: ugly. why doesnt it take a const ptr?
		return cpy > 0 ? cpy - 1 : 0;
	}

	auto unrefNodelete() const { return --std::get<0>(members_); }
	unsigned int referenceCount() const { return std::get<0>(members_).load(); }
};

///\ingroup utility
///\brief Smart pointer class for objects with built-in reference counter.
///\details If you want to make your classes reference counted types from design to be able to
///use this smart pointer, have a look at the nytl::References base class.
template<
	typename T,
	typename = decltype(std::declval<T>().ref()),
	typename = decltype(std::declval<T>().unref())>
class IntrusivePtr
{
protected:
	T* object_ {nullptr};

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
};

}

#endif //header guard
