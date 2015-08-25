#pragma once

#include <nyutil/vec.hpp>
#include <nyutil/rect.hpp>

#include <vector>

namespace nyutil
{

class region
{
protected:
	rect2ui extents_;
	std::vector<rect2i> rects_;

public:
	void add(const rect2i& r){ rects_.push_back(r); };
	void add(int x, int y, int width, int height){ rects_.push_back(rect2i(x,y,width,height)); };

	void subtract(const rect2i& r){ for(size_t i(0); i < rects_.size(); i++){ if(rects_[i].contains(r))rects_.erase(rects_.begin() + i); else if(rects_[i].intersects(r)) { std::vector<rect2i> v = rects_[i].subtract(r); for(size_t p(0); p < v.size(); p++) rects_.push_back(v[p]);}}};
	void subtract(int x, int y, int width, int height){ subtract(rect2i(x,y,width,height)); };

	bool contains(const vec2f& point) const { return 0; };
	bool contains(const rect2f& point) const { return 0; };
	bool contains(const region& point) const { return 0; };

	bool intersects(const rect2f& other) const { return 0; };
	bool intersects(const region& other) const { return 0; };
};

}
