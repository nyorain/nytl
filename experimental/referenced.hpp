#pragma once

#include <atomic>
#include <utility>

namespace nytl
{

///\ingroup utility
class Referenced
{
protected:
	mutable std::atomic<unsigned int> referenceCount_ {0};

protected:
	Referenced() = default;
	virtual ~Referenced() = default; 

public:
	void ref() const { referenceCount_++; }
	void unref() const { referenceCount_--; if(referenceCount_ == 0) delete this; }
	void unrefNodelete() const { referenceCount_--; }

	unsigned int referenceCount() const { return referenceCount_.load(); }
};

///\ingroup utility
template<typename T>
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
