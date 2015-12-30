/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <unordered_map>
#include <memory>
#include <nytl/make_unique.hpp>

namespace nytl
{

///Default cache base class used by the multiCache template.
class cache
{
public:
	virtual ~cache() noexcept = default;
	virtual std::unique_ptr<cache> cacheClone() const = 0;
};

///\brief Helper template that make deriving from cache easier.
///\details Implements the pure virtual cache::cacheClione() function depending on the given Derived 
///class template paremeter.
template<typename Derived>
class cacheBase : public cache
{
public:
    virtual std::unique_ptr<cache> cacheClone() const override 
		{ return make_unique<Derived>(static_cast<const Derived&>(*this)); }
};

///\brief Base class for classes that carry associated cache objects.
///\details Objects of classes dervied from multiCache are able to hold multiple associated cache 
//objects that have Base as common base class and can be individually accessed by a given key.
///Copying one multiCache object does clone all its cache objects, it may be therefore an
///expensive operation. If the multiCache object is move-constructed or move-assigned no cache
///object has usually to be moved or copied.
template<typename Key, typename Base = cache>
class multiCache
{
protected:
	template<unsigned int id> friend class cacheAccessor;
	mutable std::unordered_map<Key, std::unique_ptr<Base>> cache_; //unordered_map with ids?

protected:
	///This function clears all cache objects and should be called whenever the object is changed
	///in a way that invalidates its cache objects.
	void invalidateCache() const
	{
		cache_.clear();
	}

public:
	multiCache() noexcept = default;
	~multiCache() noexcept = default;

	multiCache(const multiCache& other) : cache_() 
		{ for(auto& c : other.cache_) cache_[c->first] = c->second->cacheClone(); }
	multiCache(multiCache&& other) noexcept : cache_(std::move(other.cache_)) {}

	multiCache& operator=(const multiCache& other) 
	{ 
		cache_.clear(); 
		for(auto& c : other.cache_) 
			cache_[c->first] = c->second->cacheClone(); 
		return *this; 
	}

	multiCache& operator=(multiCache&& other) noexcept 
		{ cache_ = std::move(other.cache_); return *this; }

	///Gets a cache object pointer for a given key if existent.
	///\return The associated cache for a given key, nullptr if none is found for that key.
	const Base* getCache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.end())
			return it->second.get();

        return nullptr;
	}

	///Stores a cache object with for a given key. If there exists already some cache object for
	///the given key it will be replaced. 
	///\return A reference to the moved cache object.
	Base& storeCache(const Key& id, std::unique_ptr<Base> c) const
	{
		auto ret = *c;

        auto it = cache_.find(id);
        if(it != cache_.end())
        {
            it->second = std::move(c);
        }
        else
        {
            cache_[id] = std::move(c);
        }

		return c;
	}

	///Clears the set cache object for the given key. 
	///\return 1 if an found cache object was cleared, 0 otherwise.
	bool resetCache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.cend())
		{
			cache_.erase(it);
			return 1;
		}

        return 0;
	}
};

}
