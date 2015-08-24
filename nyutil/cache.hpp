#pragma once

#include <memory>
#include <vector>

namespace nyutil
{

//if a multicacher object is copied, both object will have pointers to the SAME cache objects.
//so everytime a cache is changed, a new cache object has to be allocated and stored, that is why the cache pointer objects are const
//you should not modify cache that is already stored, because it can be stored in multiple cachers
//{done because copying of multicacher objects will prob. happen often, while cache changing should not happen that often}

//cache
class cache
{
public:
	virtual ~cache() = default;
	virtual unsigned int getID() const = 0;
};


//multiCacher
//object with which multiple cache objects can be associated
class multiCacher
{
template<unsigned int id> friend class cacheAccessor;

protected:
	mutable std::vector<std::shared_ptr<const cache>> cache_; //unordered_map with ids?

	multiCacher() = default;
	~multiCacher() = default;

	multiCacher(const multiCacher& other) : cache_(other.cache_) {}
	multiCacher(multiCacher&& other) : cache_(std::move(other.cache_)) {}

	multiCacher& operator=(const multiCacher& other) { cache_ = other.cache_; return *this; }
	multiCacher& operator=(multiCacher&& other) { cache_ = std::move(other.cache_); return *this; }

	const cache* getCache(unsigned int id) const //use std::shared_ptr ?
	{
        for(auto& ch : cache_)
        {
            if(ch->getID() == id)
                return ch.get();
        }

        return nullptr;
	}

	void store(std::shared_ptr<const cache> c, unsigned int id) const
	{
        for(auto& ch : cache_)
        {
            if(ch->getID() == id)
            {
                ch = std::move(c);
                return;
            }
        }

        cache_.push_back(std::move(c));
	}

	bool reset(unsigned int id) const
	{
        for(auto it = cache_.cbegin(); it != cache_.cend(); ++it)
        {
            if((*it)->getID() == id)
            {
                cache_.erase(it);
                return 1;
            }
        }

        return 0;
	}

	void invalidate() const
	{
        cache_.clear();
	}
};

//cache access
//class which wants to associate cache with objects
template<unsigned int id> class cacheAccessor
{
protected:
	void storeCache(const multiCacher& cacher, std::shared_ptr<const cache> obj)
	{
        cacher.store(std::move(obj), id);
	}
	bool resetCache(const multiCacher& cacher) //returns if existing cache was reset
	{
        return cacher.reset(id);
	}
	const cache* getCache(const multiCacher& cacher)
	{
        return cacher.getCache(id);
	}
};

}
