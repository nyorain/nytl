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

//cache base class
class cache
{
public:
	virtual ~cache() noexcept = default;
	virtual std::unique_ptr<cache> cacheClone() const = 0;
};

//cache derive helper class
template<typename Derived>
class cacheBase : public cache
{
public:
    virtual std::unique_ptr<cache> cacheClone() const override { return make_unique<Derived>(static_cast<const Derived&>(*this)); }
};

//multiCacher
//object with which multiple cache objects can be associated
template<typename Key, typename Base = cache>
class multiCache
{
template<unsigned int id> friend class cacheAccessor;

protected:
	mutable std::unordered_map<Key, std::unique_ptr<Base>> cache_; //unordered_map with ids?
	void invalidateCache() const
	{
		cache_.clear();
	}

public:
	multiCache() noexcept = default;
	~multiCache() noexcept = default;

	multiCache(const multiCache& other) : cache_() { for(auto& c : other.cache_) cache_[c->first] = c->second->cacheClone(); }
	multiCache(multiCache&& other) noexcept : cache_(std::move(other.cache_)) {}

	multiCache& operator=(const multiCache& other) { cache_.clear(); for(auto& c : other.cache_) cache_[c->first] = c->second->cacheClone(); return *this; }
	multiCache& operator=(multiCache&& other) noexcept { cache_ = std::move(other.cache_); return *this; }


	const Base* getCache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.end())
			return it->second.get();

        return nullptr;
	}

	void storeCache(const Key& id, std::unique_ptr<Base> c) const
	{
        auto it = cache_.find(id);
        if(it != cache_.end())
        {
            it->second = std::move(c);
        }
        else
        {
            cache_[id] = std::move(c);
        }
	}

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
