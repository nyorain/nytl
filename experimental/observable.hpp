#pragma once

#include <vector>
#include <algorithm>
#include <atomic>
#include <mutex>

namespace nytl
{

class Observable;

///\ingroup utility
class Observer
{
	friend class Observable;
	virtual void destruction(Observable&) = 0;
};

///\ingroup utility
class Observable
{
protected:
	std::vector<Observer*> observer_;
	std::mutex mutex_;

public:
	~Observable()
	{
		//no lock guard needed. Undefined behavior if destructor and accesing run at the same time
		for(auto& obs : observer_)
			obs->destruction(*this);
	}

	void addObserver(Observer& obs)
	{
		std::lock_guard<std::mutex>(mutex_);
		observer_.push_back(&obs);
	}
	bool removeObserver(Observer& obs)
	{
		std::lock_guard<std::mutex>(mutex_);
		return (observer_.cend() == std::remove(observer_.cbegin(), observer_.cend(), &obs));
	}
	bool moveObserver(Observer& oldone, Observer& newone)
	{
		std::lock_guard<std::mutex>(mutex_);
		auto it = std::find(observer_.begin(), observer_.end(), &oldone);
		if(it == observer_.cend()) return 0;
		*it = &newone;
		return 1;
	}
};

///\ingroup utility
template <typename T>
class ObservingPtr : public Observer
{
private:
	std::atomic<T*> object_ {nullptr};
	virtual void destruction() override { object_ = nullptr; }

public:
	ObservingPtr() = default;
	ObservingPtr(T* obj) : object_(obj) { if(object_) object_->addObserver(*this); }
	ObservingPtr(T& obj) : object_(&obj) { object_->addObserver(*this); }
	~ObservingPtr(){ if(object_) object_->removeObserver(*this); }

	ObservingPtr(const ObservingPtr& other) : object_(other.object_)
		{ object_->addObserver(*this); }
	ObservingPtr& operator=(const ObservingPtr& other)
		{ reset(other.object_); return *this; }

	ObservingPtr(ObservingPtr&& other) noexcept : object_(other.object_)
		{ if(object_) object_->moveObserver(other, *this); other.object_ = nullptr; }
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

	T* get() { return object_; }

	T& operator*(){ return *object_; }
	const T& operator*() const { return *object_; }

	T* operator->(){ return object_; }
	const T* operator->() const { return object_; }

	operator bool() const { return (object_ != nullptr); }
	void swap(ObservingPtr& other) noexcept 
	{
		if(object_) object_->moveObserver(*this, other);
		if(other.object_) other.object_->moveObserver(other, *this);
		std::swap(object_, other.object_);
	}
};

}
