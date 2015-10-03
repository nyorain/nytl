#pragma once

#include <vector>
#include <memory>

namespace nyutil
{

//cache base class
class cache
{
public:
	virtual ~cache() noexcept = default;

	virtual unsigned int cacheID() const = 0;
	virtual std::unique_ptr<cache> cacheClone() const = 0;
};

//cache derive helper class
template<typename Derived, unsigned int id, bool copyable = 1>
class cacheBase : public cache
{
public:
    virtual unsigned int cacheID() const override { return id; }
    virtual std::unique_ptr<cache> cacheClone() const override { return new Derived(static_cast<Derived&>(*this)); }
};

//multiCacher
//object with which multiple cache objects can be associated
class multiCacher
{
template<unsigned int id> friend class cacheAccessor;

protected:
	mutable std::vector<std::unique_ptr<cache>> cache_; //unordered_map with ids?

protected:
	multiCacher() noexcept = default;
	~multiCacher() noexcept = default;

	multiCacher(const multiCacher& other) : cache_() { for(auto& c : other.cache_) cache_.push_back(c->cacheClone()); } //sth. with reserve for perfomrance?
	multiCacher(multiCacher&& other) noexcept : cache_(std::move(other.cache_)) {}

	multiCacher& operator=(const multiCacher& other) { cache_.clear(); for(auto& c : other.cache_) cache_.push_back(c->cacheClone()); return *this; }
	multiCacher& operator=(multiCacher&& other) noexcept { cache_ = std::move(other.cache_); return *this; }

	const cache* getCache(unsigned int id) const
	{
        for(auto& ch : cache_)
        {
            if(ch->cacheID() == id)
                return ch.get();
        }

        return nullptr;
	}

	void store(std::unique_ptr<cache> c) const
	{
        for(auto& ch : cache_)
        {
            if(ch->cacheID() == c->cacheID())
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
            if((*it)->cacheID() == id)
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

//class which wants to associate cache with objects
//cache access
template<unsigned int id> class cacheAccessor
{
protected:
	void storeCache(const multiCacher& cacher, std::unique_ptr<cache> obj)
	{
	    //if(id != obj->cacheID()) ERROR
        cacher.store(std::move(obj));
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
