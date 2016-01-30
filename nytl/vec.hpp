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

///\file
///\brief Includes the Vec template class as well as Vec typedefs and utility functions.

#pragma once

#include <nytl/tmp.hpp>
#include <nytl/scalar.hpp>
#include <nytl/integer_sequence.hpp>
#include <nytl/misc.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <limits>
#include <stdexcept>

namespace nytl
{


template<size_t D, typename T> class Vec;

//typedefs
///\{
///Available Vec typedefs
template<typename T = float> using Vec2 = Vec<2, T>;
template<typename T = float> using Vec3 = Vec<3, T>;
template<typename T = float> using Vec4 = Vec<4, T>;

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2ui;
typedef Vec2<double> Vec2d;
typedef Vec2<char> Vec2c;
typedef Vec2<unsigned char> Vec2uc;
typedef Vec2<long> Vec2l;
typedef Vec2<unsigned long> Vec2ul;
typedef Vec2<bool> Vec2b;

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<unsigned int> Vec3ui;
typedef Vec3<double> Vec3d;
typedef Vec3<char> Vec3c;
typedef Vec3<unsigned char> Vec3uc;
typedef Vec3<long> Vec3l;
typedef Vec3<unsigned long> Vec3ul;
typedef Vec3<bool> Vec3b;

typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;
typedef Vec4<unsigned int> Vec4ui;
typedef Vec4<double> Vec4d;
typedef Vec4<char> Vec4c;
typedef Vec4<unsigned char> Vec4uc;
typedef Vec4<long> Vec4l;
typedef Vec4<unsigned long> Vec4ul;
typedef Vec4<bool> Vec4b;
///\}

//better use dynmiacSize = 0? 
//error messages with this one are kinda disgusting

///Constant that can be used as dimension parameter for the Vector to make its
///size variable (like std::vector), while still oferring all mathematical
///operators.
///You have to include <nytl/dynVec.hpp> to make it work.
constexpr std::size_t dynamicSize = std::numeric_limits<std::size_t>::max();


//Can be used if a parameter should be a Vec for more than one value or a scalar for exactly one.
template<std::size_t D, typename P> struct VecScalar { using type = Vec<D, P>; };
template<typename P> struct VecScalar<1, P> { using type = P; };
template<typename P> struct VecScalar<0, P> { };


//For serveral operations with/for Vec and its specializations, parameters are passed by by-copy 
//instead of by-reference. This have to be done because otherwise you could not perform 
//certain operations of the Vector in realtion to its current state.
//Example: v /= v[0]. If v[0] is passed as reference here, the components of the Vector are
//devided by difference values, since v[0] changes during the operation. If we pass it as
//copy instead, it might be more expensive, but we get at least the expected results.

///\brief The Vec class represents a sized group of \c D values of type \c T.
///\ingroup math
template<std::size_t D, typename T> class Vec
{
public:
    using value_type = T;
    constexpr static std::size_t dim = D;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using Vec_type = Vec<D, T>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    value_type data_[dim] {};

public:
    template<typename... Args, typename = typename
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>,
				typename type_tuple<value_type, dim>::type
			>::value
		>::type>
    Vec(Args&&... args) noexcept : data_{value_type(args)...} {}
	explicit Vec(size_type) noexcept {}

    Vec() noexcept = default;
    ~Vec() noexcept = default;

    Vec(const Vec_type& lhs) noexcept = default;
    Vec_type& operator=(const Vec_type& lhs) noexcept = default;

    Vec(Vec_type&& lhs) noexcept = default;
    Vec_type& operator=(Vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> Vec_type& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> Vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> Vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> Vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> Vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> Vec_type& operator %=(OT lhs){ 
		for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> Vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> Vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> Vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> Vec_type& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> Vec_type& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    Vec_type operator-() const 
		{ Vec_type ret{}; for(size_t i(0); i < dim; i++) ret[i] -= (*this)[i]; return ret; }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if< !std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator Vec<OD, ot>() const 
	{ 
		Vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<std::size_t psize>
	Vec<psize, T> subVec(std::size_t position = 0) const
	{
		auto ret = Vec<psize, T>{};
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(std::size_t position = 0, std::size_t psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return data_; }
    pointer data() noexcept { return data_; }

    void fill(const value_type& val) { for(auto& v : data_) v = val; }

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return begin() + dim; }
    const_iterator end() const noexcept { return begin() + dim; }
    const_iterator cend() const noexcept { return begin() + dim; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return data_[i]; }
    const_reference operator[](size_type i) const { return data_[i]; }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data_[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data_[i]; }

    reference front() noexcept { return data_[0]; }
    const_reference front() const noexcept { return data_[0]; }

    reference back() noexcept { return data_[dim - 1]; }
    const_reference back() const noexcept { return data_[dim - 1]; }

	void swap(Vec_type& other){ std::swap(data_, other.data_); }
};

template<typename T> class Vec<2, T>
{
public:
    using value_type = T;
    constexpr static size_t dim = 2;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator =  const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using Vec_type = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
	value_type x;
 	value_type y;

public:
	Vec(value_type a, value_type b) noexcept : x(a), y(b) {}
	explicit Vec(size_type) noexcept {}

	Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const Vec_type& lhs) = default;
	Vec_type& operator=(const Vec_type& lhs) = default;

    Vec(Vec_type&& lhs) noexcept = default;
    Vec_type& operator=(Vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> Vec_type& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> Vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> Vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> Vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> Vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> Vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> Vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> Vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> Vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    Vec_type operator-() const { return Vec_type(-x, -y); }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type>
	operator Vec<OD, ot>() const 
	{ 
		Vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<std::size_t psize>
	Vec<psize, T> subVec(std::size_t position = 0) const
	{
		auto ret = Vec<psize, T>{};
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(std::size_t position = 0, std::size_t psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; }

    iterator begin() noexcept { return &x; }
    const_iterator begin() const noexcept { return &x; }
    const_iterator cbegin() const noexcept { return &x; }

    iterator end() noexcept { return &y + 1; }
    const_iterator end() const noexcept { return &y + 1; }
    const_iterator cend() const noexcept { return &y + 1; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return *(data() + i); }
    const_reference operator[](size_type i) const { return *(data() + i); }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return *(data() + i); }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return *(data() + i); }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return y; }
    const_reference back() const noexcept { return y; }

	void swap(Vec_type& other){ std::swap(x, other.x); std::swap(y, other.y); }
};


template<typename T> class Vec<3, T>
{
public:
    using value_type = T;
    constexpr static size_t dim = 3;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using Vec_type = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    value_type x;
    value_type y;
    value_type z;

public:
	Vec(value_type a, value_type b, value_type c) noexcept : x(a), y(b), z(c) {}
	explicit Vec(size_type) noexcept {}

    Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const Vec_type& lhs) noexcept = default;
	Vec_type& operator=(const Vec_type& lhs) noexcept = default;

    Vec(Vec_type&& lhs) noexcept = default;
    Vec_type& operator=(Vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> Vec_type& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> Vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> Vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> Vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> Vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> Vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> Vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> Vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> Vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    Vec_type operator-() const { return Vec_type(-x, -y, -z); }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator Vec<OD, ot>() const 
	{ 
		Vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<std::size_t psize>
	Vec<psize, T> subVec(std::size_t position = 0) const
	{
		auto ret = Vec<psize, T>{};
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(std::size_t position = 0, std::size_t psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; }

    iterator begin() noexcept { return &x; }
    const_iterator begin() const noexcept { return &x; }
    const_iterator cbegin() const noexcept { return &x; }

    iterator end() noexcept { return &z + 1; }
    const_iterator end() const noexcept { return &z + 1; }
    const_iterator cend() const noexcept { return &z + 1; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return *(data() + i); }
    const_reference operator[](size_type i) const { return *(data() + i); }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return z; }
    const_reference back() const noexcept { return z; }

	void swap(Vec_type& other)
		{ std::swap(x, other.x); std::swap(y, other.y); std::swap(z, other.z); }

    //custom
    Vec2<value_type> xy() const noexcept { return Vec2<value_type>(x,y); }
    Vec2<value_type> yz() const noexcept { return Vec2<value_type>(y,z); }
    Vec2<value_type> xz() const noexcept { return Vec2<value_type>(x,z); }
};


template<typename T> class Vec<4, T>
{
public:
    using value_type = T;
    constexpr static size_t dim = 4;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using Vec_type = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    value_type x;
    value_type y;
    value_type z;
    value_type w;

public:
	Vec(value_type a, value_type b, value_type c, value_type d) noexcept : x(a), y(b), z(c), w(d) {}
	explicit Vec(size_type) noexcept {}

    Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const Vec_type& lhs) noexcept = default;
	Vec_type& operator=(const Vec_type& lhs) noexcept = default;

    Vec(Vec_type&& lhs) noexcept = default;
    Vec_type& operator=(Vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> Vec_type& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> Vec_type& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> Vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> Vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> Vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> Vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> Vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> Vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> Vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> Vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    Vec_type operator-() const { return  Vec_type(-x, -y, -z, -w); }

    template<std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator Vec<OD, ot>() const
	{ 
		Vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 

		return ret; 
	}

	template<std::size_t psize>
	Vec<psize, T> subVec(std::size_t position = 0) const
	{
		auto ret = Vec<psize, T>{};
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(std::size_t position = 0, std::size_t psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; w = val; }

    iterator begin() noexcept { return &x; }
    const_iterator begin() const noexcept { return &x; }
    const_iterator cbegin() const noexcept { return &x; }

    iterator end() noexcept { return &w + 1; }
    const_iterator end() const noexcept { return &w + 1; }
    const_iterator cend() const noexcept { return &w + 1; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return *(data() + i); }
    const_reference operator[](size_type i) const { return *(data() + i); }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return w; }
    const_reference back() const noexcept { return w; }

	void swap(Vec_type& other) { std::swap(x, other.x); std::swap(y, other.y); 
		std::swap(z, other.z); std::swap(w, other.w); }

    //custom
    Vec2<T> xy() const noexcept { return Vec2<T>(x,y); }
    Vec2<T> xz() const noexcept { return Vec2<T>(x,z); }
    Vec2<T> xw() const noexcept { return Vec2<T>(x,w); }
    Vec2<T> yz() const noexcept { return Vec2<T>(y,z); }
    Vec2<T> yw() const noexcept { return Vec2<T>(y,w); }
    Vec2<T> zw() const noexcept { return Vec2<T>(z,w); }

    Vec3<T> xyz() const noexcept { return Vec3<T>(x,y,z); }
    Vec3<T> xyw() const noexcept { return Vec3<T>(x,y,w); }
    Vec3<T> xzw() const noexcept { return Vec3<T>(x,z,w); }
    Vec3<T> yzw() const noexcept { return Vec3<T>(y,z,w); }
};

template<std::size_t D, typename T> constexpr std::size_t Vec<D, T>::dim;
template<typename T> constexpr std::size_t Vec<2, T>::dim;
template<typename T> constexpr std::size_t Vec<3, T>::dim;
template<typename T> constexpr std::size_t Vec<4, T>::dim;

//invalid specialization - therefore not specified
template<typename T> class Vec<0, T>;

//reference Vec, include <nytl/refVec.hpp> to make those work!
template<std::size_t D, typename T> class Vec<D, T&>;
template<typename T> class Vec<2, T&>;
template<typename T> class Vec<3, T&>;
template<typename T> class Vec<4, T&>;

//Dynamic storage Vec, include <nytl/dynVec.hpp> to make this work!
template<typename T> class Vec<dynamicSize, T>;
template<typename T> class Vec<dynamicSize, T&>; //where to put this? <nytl/dynRefVec>? TODO

//operators/utility
#include <nytl/bits/vec.inl>

}


//////////////////////////////////////////////////////////////////////////////////////////////
/// \class nytl::Vec
/// \ingroup math
///
/// The template parameter D defines the dimension of the Vector.
/// The template parameter T defines the type of the Vectors components.
///
/// In most of the cases you do not have to care about the template parameters because there are
/// typedefs for the common used types, e.g.:
/// \li nytl::Vec2f, nytl::Vec3f and nytl::Vec4f for float Vecs
/// \li nytl::Vec2i, nytl::Vec3i and nytl::Vec4i for int Vecs
///
/// There are much more Vec typedefs. All of them are named after a common pattern:
/// \c "nytl::Vec" + \c D + \c T with \c D = {2, 3, 4} and \c T of: 
/// \li f for float
/// \li i for int
/// \li ui for unsigned int 
/// \li d for double
/// \li b for bool
/// \li c for char
/// \li uc for unsigned char
/// \li l for long
/// \li ul for unsigned long
///
/// Additionally there are nytl::Vec2<T>, nytl::Vec3<T> and nytl::Vec4<T> typedefs.
/// There are Vector specializations for 2;3 and 4 dimensional Vectors, as well as for a dynamic
/// dimension (indicated by nytl::dynamicSize as D) or a reference type.
/// All specializations behave like normal Vecs and provide the same operations. To make
/// the extra specializations work you have to include the corresponding headers,
/// <nytl/refVec.hpp> for reference-typed Vectors or <nytl/dynVec.hpp> for dynamic-sized Vectors.
///
/// Except for the special dimension dynamicSize which make the Vec object more like a
/// std::vector with the possibility to add and remove components, Vec objects are usually
/// fixed-size and therefore entirely allocated on the stack. Fixed-size Vec specializations
/// are always pod classes which means they can be safely casted to other pod types (one can
/// e.g. safely cast a Vec3<Vec2f> into a mat32f).
/// 
/// In difference to std::array it is meant like a representation of a mathematical Vector, it
/// also has specializations for Vec2, Vec3 and Vec4 which mache dealing with them easier.
/// The available functions for dealing with Vec and the design of Vec itself is closely
/// related to the design of Vec in glsl (free functional operators, mathematical operators)
/// while still oferring modern c++ features that make using them easier (e.g. iterators, tmp 
/// constructors or conversion operators).
/// This is the most important class for all further mathematical classes and operations in nytl
/// since it can be used for storing position, size or as a general fixed- (or even dynamic-) sized
/// container.
////////////////////////////////////////////////////////////////////////////////////////////////// 

