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
///\brief Includes the vec template class as well as vec typedefs and utility functions.

#pragma once

#include <nytl/tmp.hpp>
#include <nytl/constants.hpp>
#include <nytl/integer_sequence.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <limits>

namespace nytl
{


template<size_t D, typename T> class vec;

//typedefs
///\brief Typedefs a vector with 2 components.
template<typename T = float> using vec2 = vec<2, T>;
template<typename T = float> using vec3 = vec<3, T>;
template<typename T = float> using vec4 = vec<4, T>;

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;
typedef vec2<unsigned int> vec2ui;
typedef vec2<double> vec2d;
typedef vec2<char> vec2c;
typedef vec2<unsigned char> vec2uc;
typedef vec2<long> vec2l;
typedef vec2<unsigned long> vec2ul;
typedef vec2<bool> vec2b;

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3<unsigned int> vec3ui;
typedef vec3<double> vec3d;
typedef vec3<char> vec3c;
typedef vec3<unsigned char> vec3uc;
typedef vec3<long> vec3l;
typedef vec3<unsigned long> vec3ul;
typedef vec3<bool> vec3b;

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<unsigned int> vec4ui;
typedef vec4<double> vec4d;
typedef vec4<char> vec4c;
typedef vec4<unsigned char> vec4uc;
typedef vec4<long> vec4l;
typedef vec4<unsigned long> vec4ul;
typedef vec4<bool> vec4b;

//better use dynmiacSize = 0? 
//error messages with this one are kinda disgusting

///Constant that can be used as dimension parameter for the vector to make its
///size variable (like std::vector), while still oferring all mathematical
///operators.
///You have to include <nytl/dynVec.hpp> to make it work.
constexpr std::size_t dynamicSize = std::numeric_limits<std::size_t>::max();


//For serveral operations with/for vec and its specializations, parameters are passed by by-copy 
//instead of by-reference. This have to be done because otherwise you could not perform 
//certain operations of the vector in realtion to its current state.
//Example: v /= v[0]. If v[0] is passed as reference here, the components of the vector are
//devided by difference values, since v[0] changes during the operation. If we pass it as
//copy instead, it might be more expensive, but we get at least the expected results.

///\brief The vec class represents a sized group of \c D values of type \c T.
///\ingroup math
template<size_t D, typename T> class vec
{
public:
    using value_type = T;
    constexpr static size_t dim = D;

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

    using vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    value_type data_[dim];

public:
    template<typename... Args, typename = typename
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>,
				typename type_tuple<value_type, dim>::type
			>::value
		>::type>
    vec(Args&&... args) noexcept : data_{std::forward<Args>(args)...} {}
	vec(size_type) noexcept {}

    vec() noexcept = default;
    ~vec() noexcept = default;

    vec(const vec_type& lhs) noexcept = default;
    vec_type& operator=(const vec_type& lhs) noexcept = default;

    vec(vec_type&& lhs) noexcept = default;
    vec_type& operator=(vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> vec_type& operator +=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator -=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator *=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator /=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator %=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator |=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator ^=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator &=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator >>=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> vec_type& operator <<=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> vec_type& operator %=(OT lhs){ 
		for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> vec_type& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> vec_type& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    vec_type operator-() const 
		{ vec_type ret{}; for(size_t i(0); i < dim; i++) ret[i] -= (*this)[i]; return ret; }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if< !std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator vec<OD, ot>() const 
	{ 
		vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
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
		throw std::out_of_range("nytl::vec::at: out of range"); return data_[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::vec::at: out of range"); return data_[i]; }

    reference front() noexcept { return data_[0]; }
    const_reference front() const noexcept { return data_[0]; }

    reference back() noexcept { return data_[dim - 1]; }
    const_reference back() const noexcept { return data_[dim - 1]; }

	void swap(vec_type& other){ std::swap(data_, other.data_); }
};

template<typename T> class vec<2, T>
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

    using vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
	value_type x;
 	value_type y;

public:
	vec(value_type a, value_type b) noexcept : x(a), y(b) {}
	vec(size_type) noexcept {}

	vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& lhs) noexcept = default;
	vec_type& operator=(const vec_type& lhs) noexcept = default;

    vec(vec_type&& lhs) noexcept = default;
    vec_type& operator=(vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> vec_type& operator +=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator -=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator *=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator /=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator %=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator |=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator ^=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator &=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    vec_type operator-() const { return vec_type(-x, -y); }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type>
	operator vec<OD, ot>() const 
	{ 
		vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
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
		throw std::out_of_range("nytl::vec::at: out of range"); return *(data() + i); }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::vec::at: out of range"); return *(data() + i); }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return y; }
    const_reference back() const noexcept { return y; }

	void swap(vec_type& other){ std::swap(x, other.x); std::swap(y, other.y); }
};


template<typename T> class vec<3, T>
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

    using vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    value_type x;
    value_type y;
    value_type z;

public:
	vec(value_type a, value_type b, value_type c) noexcept : x(a), y(b), z(c) {}
	vec(size_type) noexcept {}

    vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& lhs) noexcept = default;
	vec_type& operator=(const vec_type& lhs) noexcept = default;

    vec(vec_type&& lhs) noexcept = default;
    vec_type& operator=(vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> vec_type& operator +=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator -=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator *=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator /=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator %=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator |=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator ^=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator &=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    vec_type operator-() const { return vec_type(-x, -y, -z); }

    template <std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator vec<OD, ot>() const 
	{ 
		vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 
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
		throw std::out_of_range("nytl::vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return z; }
    const_reference back() const noexcept { return z; }

	void swap(vec_type& other)
		{ std::swap(x, other.x); std::swap(y, other.y); std::swap(z, other.z); }

    //custom
    vec2<value_type> xy() const noexcept { return vec2<value_type>(x,y); }
    vec2<value_type> yz() const noexcept { return vec2<value_type>(y,z); }
    vec2<value_type> xz() const noexcept { return vec2<value_type>(x,z); }
};


template<typename T> class vec<4, T>
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

    using vec_type = vec<dim, value_type>;

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
	vec(value_type a, value_type b, value_type c, value_type d) noexcept : x(a), y(b), z(c), w(d) {}
	vec(size_type) noexcept {}

    vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& lhs) noexcept = default;
	vec_type& operator=(const vec_type& lhs) noexcept = default;

    vec(vec_type&& lhs) noexcept = default;
    vec_type& operator=(vec_type&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> vec_type& operator +=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator -=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator *=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator /=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator %=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator |=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator ^=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <std::size_t OD, typename ot> vec_type& operator &=(const vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> vec_type& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> vec_type& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> vec_type& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> vec_type& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> vec_type& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> vec_type& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> vec_type& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> vec_type& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    vec_type operator-() const { return  vec_type(-x, -y, -z, -w); }

    template<std::size_t OD, typename ot, typename = 
		typename std::enable_if<!std::is_reference<ot>::value && OD != dynamicSize>::type> 
	operator vec<OD, ot>() const
	{ 
		vec<OD, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < min(OD, dim); i++) 
			ret[i] = (*this)[i]; 

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
		throw std::out_of_range("nytl::vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return w; }
    const_reference back() const noexcept { return w; }

	void swap(vec_type& other) { std::swap(x, other.x); std::swap(y, other.y); 
		std::swap(z, other.z); std::swap(w, other.w); }

    //custom
    vec2<T> xy() const noexcept { return vec2<T>(x,y); }
    vec2<T> xz() const noexcept { return vec2<T>(x,z); }
    vec2<T> xw() const noexcept { return vec2<T>(x,w); }
    vec2<T> yz() const noexcept { return vec2<T>(y,z); }
    vec2<T> yw() const noexcept { return vec2<T>(y,w); }
    vec2<T> zw() const noexcept { return vec2<T>(z,w); }

    vec3<T> xyz() const noexcept { return vec3<T>(x,y,z); }
    vec3<T> xyw() const noexcept { return vec3<T>(x,y,w); }
    vec3<T> xzw() const noexcept { return vec3<T>(x,z,w); }
    vec3<T> yzw() const noexcept { return vec3<T>(y,z,w); }
};

template<std::size_t D, typename T> constexpr std::size_t vec<D, T>::dim;
template<typename T> constexpr std::size_t vec<2, T>::dim;
template<typename T> constexpr std::size_t vec<3, T>::dim;
template<typename T> constexpr std::size_t vec<4, T>::dim;

//invalid specialization - therefore not specified
template<typename T> class vec<0, T>;

//reference vec, include <nytl/refVec.hpp> to make those work!
template<std::size_t D, typename T> class vec<D, T&>;
template<typename T> class vec<2, T&>;
template<typename T> class vec<3, T&>;
template<typename T> class vec<4, T&>;

//Dynamic storage vec, include <nytl/dynVec.hpp> to make this work!
template<typename T> class vec<dynamicSize, T>;
template<typename T> class vec<dynamicSize, T&>; //where to put this? <nytl/dynRefVec>? TODO

//operators/utility
#include <nytl/bits/vec.inl>

}


//////////////////////////////////////////////////////////////////////////////////////////////
/// \class nytl::vec
/// \ingroup math
///
/// The template parameter D defines the dimension of the vector.
/// The template parameter T defines the type of the vectors components.
///
/// In most of the cases you do not have to care about the template parameters because there are
/// typedefs for the common used types, e.g.:
/// \li nytl::vec2f, nytl::vec3f and nytl::vec4f for float vecs
/// \li nytl::vec2i, nytl::vec3i and nytl::vec4i for int vecs
///
/// There are much more vec typedefs. All of them are named after a common pattern:
/// \c "nytl::vec" + \c D + \c T with \c D = {2, 3, 4} and \c T of: 
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
/// Additionally there are nytl::vec2<T>, nytl::vec3<T> and nytl::vec4<T> typedefs.
/// There are vector specializations for 2;3 and 4 dimensional vectors, as well as for a dynamic
/// dimension (indicated by nytl::dynamicSize as D) or a reference type.
/// All specializations behave like normal vecs and provide the same operations. To make
/// the extra specializations work you have to include the corresponding headers,
/// <nytl/refVec.hpp> for reference-typed vectors or <nytl/dynVec.hpp> for dynamic-sized vectors.
///
/// Except for the special dimension dynamicSize which make the vec object more like a
/// std::vector with the possibility to add and remove components, vec objects are usually
/// fixed-size and therefore entirely allocated on the stack. Fixed-size vec specializations
/// are always pod classes which means they can be safely casted to other pod types (one can
/// e.g. safely cast a vec3<vec2f> into a mat32f).
/// 
/// In difference to std::array it is meant like a representation of a mathematical vector, it
/// also has specializations for vec2, vec3 and vec4 which mache dealing with them easier.
/// The available functions for dealing with vec and the design of vec itself is closely
/// related to the design of vec in glsl (free functional operators, mathematical operators)
/// while still oferring modern c++ features that make using them easier (e.g. iterators, tmp 
/// constructors or conversion operators).
/// This is the most important class for all further mathematical classes and operations in nytl
/// since it can be used for storing position, size or as a general fixed- (or even dynamic-) sized
/// container.
////////////////////////////////////////////////////////////////////////////////////////////////// 

