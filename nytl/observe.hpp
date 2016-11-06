// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Base classes for making types observable or observe the lifetime of objects.

#pragma once

#ifndef NYTL_INCLUDE_OBSERVE_HPP
#define NYTL_INCLUDE_OBSERVE_HPP

#include <vector>
#include <algorithm>
#include <atomic>
#include <mutex>

namespace nytl
{

class Observable;

///\ingroup utility
///Base class for classes that observe the lifetime of other Observeable objects.
class Observer
{
	friend class Observable;

	///Virtual function that will be called when the object observed by this
	///observer will be desctructed.
	virtual void observeDestruction(Observable&) = 0;
};

///\ingroup utility
///\brief Utility class to make the objects lifetime observable.
///\details Base class that can be derived from if the lifetime of objects of this class should be
///observeable by others.
///Note that this class is not threadsafe, and calls to move/add/remove Observer must interfer in
///any way.
///\sa nytl::Observer
///\sa nytl::ObservingPtr.
class Observable
{
protected:
	std::vector<Observer*> observer_;

public:
	~Observable()
	{
		//if some (weird) Observer implementations remove themself during the callback.
		auto cpy = std::move(observer_);
		for(auto& obs : cpy)
			obs->observeDestruction(*this);
	}

	void addObserver(Observer& obs)
	{
		observer_.push_back(&obs);
	}
	bool removeObserver(const Observer& obs)
	{
		for(auto i = 0u; i < observer_.size(); ++i)
		{
			if(observer_[i] == &obs)
			{
				observer_.erase(observer_.cbegin() + i);
				return true;
			}
		}

		return false;
	}
	bool moveObserver(Observer& oldone, Observer& newone)
	{
		auto it = std::find(observer_.begin(), observer_.end(), &oldone);
		if(it == observer_.cend()) return false;
		*it = &newone;
		return true;
	}
};

///Wrapper to make already defined classes observable.
template<typename T>
struct ObservableWrapper : public T, public Observable
{
	using T::T;
};

///\ingroup utility
///\brief Smart pointer class that observes the lifetime of its object.
///\details Basically a smart pointer that does always know whether the object it points to is
//alive or not. Does only work with objects of classes that are derived from nytl::Observeable.
///Semantics are related to std::unique_ptr/std::shared_ptr.
template <typename T>
class ObservingPtr : public Observer
{
public:
	ObservingPtr() = default;
	ObservingPtr(T* obj) : object_(obj) { if(object_) object_->addObserver(*this); }
	ObservingPtr(T& obj) : object_(&obj) { object_->addObserver(*this); }
	~ObservingPtr(){ if(object_) object_->removeObserver(*this); }

	ObservingPtr(const ObservingPtr& other) : object_(other.object_)
	{
		if(object_) object_->addObserver(*this);
	}
	ObservingPtr& operator=(const ObservingPtr& other)
	{
		reset(other.object_);
		return *this;
	}

	ObservingPtr(ObservingPtr&& other) noexcept : object_(other.object_)
	{
		if(object_) object_->moveObserver(other, *this);
		other.object_ = nullptr;
	}
	ObservingPtr& operator=(ObservingPtr&& other) noexcept
	{
		reset();
		object_ = other.object_;
		if(object_) object_->moveObserver(other, *this);
		other.object_ = nullptr;
		return *this;
	}

	void reset(T* obj = nullptr)
	{
		if(obj) obj->addObserver(*this);
		if(object_) object_->removeObserver(*this);
		object_ = obj;
	}
	void reset(T& obj)
	{
		obj.addObserver(*this);
		if(object_) object_->removeObserver(*this);
		object_ = &obj;
	}

	T* get() const { return object_; }
	T& operator*() const { return *object_; }
	T* operator->() const { return object_; }

	operator bool() const { return (object_ != nullptr); }
	void swap(ObservingPtr& other) noexcept
	{
		if(object_) object_->moveObserver(*this, other);
		if(other.object_) other.object_->moveObserver(other, *this);
		std::swap(object_, other.object_);
	}

private:
	T* object_ {nullptr};
	virtual void observeDestruction(Observable&) override { object_ = nullptr; }
};

}

#endif //header guard
