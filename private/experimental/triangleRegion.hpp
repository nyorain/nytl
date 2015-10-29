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

#include <nytl/vec.hpp>
#include <nytl/rect.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>

#include <vector>

//basically like region but with triangles, so it can represent ANY 2D region, normal region class is not capable of that.
//problems: -can only be used in 2D space (no general triangle type abstraction like... line->triangle->pyramid like its possible with rect
//          -way more complex to compute (nothing implemented yet, TODO!)

namespace nytl
{

template<typename prec>
class triangleRegion
{
template<typename T> friend class triangleRegion;

public:
    using value_type = prec;
    using vec_type = vec2<prec>;
    using rect_type = rect2<prec>;
    using triangle_type = triangle2<prec>;
    using region_type = region<prec>;
    using line_type = line2<prec>;

protected:
	std::vector<triangle_type> triangles_ {};

public:
    region() noexcept = default;
    ~region() noexcept = default;

    region(const region_type& other) = default;
    region& operator=(const region_type& other) = default;

    region(region_type&& other) noexcept = default;
    region& operator=(region_type&& other) noexcept = default;

    //
	void add(const triangle_type& t){ triangles_.push_back(t); cacheNeedsUpdate_ = 1; };
	void add(const rect_type& r){ auto spl = split(r); triangles_.push_back(spl[0]); triangles_.push_back(spl[1]); cacheNeedsUpdate_ = 1; };
	void add(value_type x, value_type y, value_type width, value_type height){ add(rect_type(x,y,width,height)); };
	void add(const region_type& r){ triangles_.insert(triangles_.end(), r.triangles_.begin(), r.triangles_.end()); cacheNeedsUpdate_ = 1; }

    //todo
	void subtract(const rect_type& r){ cacheNeedsUpdate_ = 1; };
	void subtract(value_type x, value_type y, value_type width, value_type height){ subtract(rect_type(x,y,width,height)); };
	void subtract(const region_type& r){ cacheNeedsUpdate_ = 1; };

	bool contains(const vec_type& point) const { return 0; };
	bool contains(const rect_type& r) const { return 0; };
	bool contains(const region_type& r) const { return 0; };
	bool contains(const line_type& l) const { return 0; };

	bool intersects(const triangle_type& r) const { return 0; };
	bool intersects(const rect_type& r) const { return 0; };
	bool intersects(const region_type& r) const { return 0; };
	bool intersects(const line_type& r) const { return 0; };

	//operator
	region& operator|=(const triangle_type& t){ add(t); return *this; }
	region& operator|=(const rect_type& r){ add(r); return *this; }
	region& operator|=(const region_type& r){ add(r); return *this; }

    region& operator&=(const triangle_type& t){  return *this; }
    region& operator&=(const rect_type& r){ return *this; }
	region& operator&=(const region_type& r){  return *this; }

    region& operator^=(const triangle_type& t){ return *this; }
    region& operator^=(const rect_type& r){ return *this; }
	region& operator^=(const region_type& r){  return *this; }

	//
	rect_type extents() const { return rect_type{}; }
	const std::vector<triangle_type>& getTriangles() const { return triangles_; }

	template<typename oprec>
	operator region<oprec>() const { region<oprec> ret; ret.triangles_.insert(ret.triangles_.end(), triangles_.begin(), triangles_.end()); return ret; }
};

}
