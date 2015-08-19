#pragma once

#include <memory>
#include <vector>

namespace nyutil
{

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
	mutable std::vector<std::unique_ptr<cache>> cache_; //unordered_map with ids?

	cache* getCache(unsigned int id) const
	{
        for(auto& ch : cache_)
        {
            if(ch->getID() == id)
                return ch.get();
        }

        return nullptr;
	}

	void store(std::unique_ptr<cache> c, unsigned int id) const
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
};

//cache access
//class which wants to associate cache with objects
template<unsigned int id> class cacheAccessor
{
protected:
	void storeCache(const multiCacher& cacher, std::unique_ptr<cache> obj)
	{
        cacher.store(std::move(obj), id);
	}
	bool resetCache(const multiCacher& cacher) //returns if existing cache was reset
	{
        return cacher.reset(id);
	}
	cache* getCache(const multiCacher& cacher)
	{
        return cacher.getCache(id);
	}
};

}
