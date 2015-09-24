#pragma once

#include <nyutil/vec.hpp>
#include <nyutil/rect.hpp>

#include <vector>

namespace nyutil
{

class region
{
protected:
    mutable bool cacheNeedsUpdate_ {0};
	mutable rect2ui extents_ {}; //cache

	std::vector<rect2i> rects_ {};

	void bakeCache() const { /*todo*/ cacheNeedsUpdate_ = 0; }

public:
    region() noexcept = default;
    ~region() noexcept = default;

    region(const region& other) = default;
    region& operator=(const region& other) = default;

    region(region&& other) noexcept = default;
    region& operator=(region&& other) noexcept = default;

    //
	void add(const rect2i& r){ rects_.push_back(r); cacheNeedsUpdate_ = 1; };
	void add(int x, int y, int width, int height){ rects_.push_back(rect2i(x,y,width,height)); cacheNeedsUpdate_ = 1; };
	void add(const region& r){ rects_.insert(rects_.end(), r.rects_.begin(), r.rects_.end()); cacheNeedsUpdate_ = 1; }

    //todo
	void subtract(const rect2i& r){ cacheNeedsUpdate_ = 1; };
	void subtract(int x, int y, int width, int height){ subtract(rect2i(x,y,width,height)); };
	void subtract(const region& r){ cacheNeedsUpdate_ = 1; };

	bool contains(const vec2f& point) const { return 0; };
	bool contains(const rect2f& point) const { return 0; };
	bool contains(const region& point) const { return 0; };

	bool intersects(const rect2f& other) const { return 0; };
	bool intersects(const region& other) const { return 0; };

	//operator
	region& operator|=(const rect2i& r){ add(r); return *this; }
	region& operator|=(const region& r){ add(r); return *this; }

    region& operator&=(const rect2i& r){ /*todo*/ return *this; }
	region& operator&=(const region& r){ /*todo*/ return *this; }

    region& operator^=(const rect2i& r){ /*todo*/ return *this; }
	region& operator^=(const region& r){ /*todo*/ return *this; }

	//
	rect2i extents() const { if(cacheNeedsUpdate_)bakeCache(); return extents_; }
};

//operators
//move needed because it is a expression, not an identifier
region operator|(region a, const region& b)
{
    return std::move(a |= b);
}

region operator|(region a, const rect2i& b)
{
    return std::move(a |= b);
}

region operator&(region a, const region& b)
{
    return std::move(a &= b);
}

region operator&(region a, const rect2i& b)
{
    return std::move(a &= b);
}

region operator^(region a, const region& b)
{
    return std::move(a ^= b);
}

region operator^(region a, const rect2i& b)
{
    return std::move(a ^= b);
}

}
