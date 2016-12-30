// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Provides a base class for reference-counted types as well as a intrusive smart pointer.

#pragma once

#ifndef NYTL_INCLUDE_REFERENCED
#define NYTL_INCLUDE_REFERENCED

#include <atomic> // std::atomic
#include <tuple> // std::tuple
#include <utility> // std::swap
#include <memory> // std::default_delete

namespace nytl {

/// Base class for reference counted objects.
/// The reference count and its operations are atomic and threadsafe.
/// \requires 'T' must be derived from this class using the CRTP idiom
/// \tparam T The derived class.
/// \tparam Deleter The deleter type for objects of this type. Makes it possible
/// to derive from this class without having to make the class virtual.
/// The References constructor takes an object of type Deleter which will be called
/// with the this pointer (casted to a const T* pointer) as soon as the reference count is
/// decreased to zero and the object should be deleted. By default, std::default_delete<T>
/// will be used that will just call delete on the object.
/// \requires Deleter must implement operator() for a const T* parameter. Implementating it for
/// a non-const T* parameter results in undefined behaviour.
/// \module utility
template<typename T, typename Deleter = std::default_delete<T>>
class Referenced {
public:
	Referenced(const Deleter& deleter = {}, unsigned int count = 0) : members_(count, deleter) {}
	~Referenced() = default;

	/// Increases the reference count.
	/// Can be called from multiple threads at the same time.
	/// \returns The reference count after the call.
	auto ref() const { return ++std::get<0>(members_); }

	/// Decreases the reference count.
	/// Calling unref more times than calling ref or using the object after the last unref results
	/// in undefined behaviour since the object may be deleted already.
	/// \returns The reference count after the call. If this is 0 the object was deleted.
	auto unref() const
	{
		// atomic decrement. It is assured that this will never be called when count is already 0
		// the const_cast is done because std::default_delete takes a non-const pointer
		auto cpy = --std::get<0>(members_);
		if(cpy == 0) std::get<1>(members_)(const_cast<T*>(static_cast<const T*>(this)));
		return cpy;
	}

	/// Decreases the reference count without checking if the object has to be deleted.
	/// Note that this function might be faster than the plain unref call so it could be
	/// preferred when the caller knows that he does not hold the last reference.
	/// Results in undefined behaviour if this function decreases the reference count to zero.
	/// \note This function has to be used with attention.
	auto unrefNodelete() const { return --std::get<0>(members_); }

	/// Returns the reference count at the moment.
	/// Note that since Referenced objects might be accessed from multiple threads, the
	/// returned value might be outdated from the beginning. It only tells that at one
	/// moment of (potentially already ended) lifetime of this object, the reference count
	/// had the returned value.
	auto referenceCount() const { return std::get<0>(members_).load(); }

protected:
	// tuple for empty class optimization, since Deleter might be stateless
	mutable std::tuple<std::atomic<unsigned int>, Deleter> members_ {};
};

/// \brief Wrapper around [nytl::Referenced]() that applies it to already existent classes.
/// \module utility
template<typename T>
struct ReferencedWrapper : public T, public Referenced<ReferencedWrapper<T>> {
	using T::T;
};

/// \brief Smart pointer class for objects with built-in reference counter.
/// \requires Type 'T' shall have 'ref' and 'unref' member function that increase/decrease its
/// reference count.
/// \module utility
template<typename T, typename = decltype(std::declval<T>().ref(), std::declval<T>().unref())>
class IntrusivePtr {
public:
	IntrusivePtr() = default;
	IntrusivePtr(T* obj) noexcept : object_(obj) { if(object_) object_->ref(); }
	IntrusivePtr(T& obj) noexcept : object_(&obj) { object_->ref(); }
	~IntrusivePtr() noexcept { if(object_) object_->unref(); }

	IntrusivePtr(const IntrusivePtr& other) noexcept : object_(other.object_)
	{
		object_->ref();
	}

	IntrusivePtr& operator=(const IntrusivePtr& other) noexcept
	{
		reset(other.object_);
		return *this;
	}

	IntrusivePtr(IntrusivePtr&& other) noexcept : object_(other.object_)
	{
		other.object_ = {};
	}

	IntrusivePtr& operator=(IntrusivePtr&& other) noexcept
	{
		if(object_) object_->unref();
		object_ = other.object_;
		other.object_ = nullptr;
		return *this;
	}

	void reset(T* obj = nullptr)
	{
		if(obj) obj->ref();
		if(object_) object_->unref();
		object_ = obj;
	}

	void reset(T& obj)
	{
		obj->ref();
		if(object_) object_->unref();
		object_ = obj;
	}

	T* get() const noexcept { return object_; }
	T* operator->() const noexcept { return object_; }
	T& operator*() const noexcept { return *object_; }

	operator bool() const noexcept { return (object_ != nullptr); }
	friend void swap(IntrusivePtr& a, IntrusivePtr& b) noexcept
	{
		std::swap(a.object_, b.object_);
	}

protected:
	T* object_ {nullptr};
};

/// \brief Can be used to allocate a new object of referenced type 'T' into an IntrusivePtr.
/// \module utility
template<typename T, typename... Args>
IntrusivePtr<T> makeIntrusive(Args&&... args) { return {new T(std::forward<Args>(args)...)}; }

}

#endif //header guard
