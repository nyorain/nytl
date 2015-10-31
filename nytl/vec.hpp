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

#include <nytl/tmp.hpp>
#include <nytl/constants.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace nytl
{


template<size_t dim, typename T> class vec;

//typedefs
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vec default
template<size_t dimension, typename T> class vec
{
public:
    using value_type = T;
    constexpr static size_t dim = dimension;

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

    using vec_type = vec<dimension, value_type>;

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

    vec() noexcept = default;
    ~vec() noexcept = default;

    vec(const vec_type& other) noexcept = default;
    vec_type& operator=(const vec_type& other) noexcept = default;

    vec(vec_type&& other) noexcept = default;
    vec_type& operator=(vec_type&& other) noexcept = default;

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator >>=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator <<=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other; return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> vec_type& operator >>=(const ot& other){ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> vec_type& operator <<=(const ot& other){ for(auto& val : *this) val <<= other; return *this; }

    vec_type operator-() const { vec_type ret{}; for(size_t i(0); i < dim; i++) ret[i] -= (*this)[i]; return ret; }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

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

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data_[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data_[i]; }

    reference front() noexcept { return data_[0]; }
    const_reference front() const noexcept { return data_[0]; }

    reference back() noexcept { return data_[dim - 1]; }
    const_reference back() const noexcept { return data_[dim - 1]; }
};

//vec specializations for vec2, vec3 and vec4
//[is pod] (? implementation defined, should be. per std it is standardLayout)
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

	vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& other) noexcept = default;
	vec_type& operator=(const vec_type& other) noexcept = default;

    vec(vec_type&& other) noexcept = default;
    vec_type& operator=(vec_type&& other) noexcept = default;

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i];  return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other;  return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other;  return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other;  return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other;  return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other;  return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other;  return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other;  return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other;  return *this; }

    vec_type operator-() const { return vec_type(-x, -y); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

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

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return *(data() + i); }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return *(data() + i); }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return y; }
    const_reference back() const noexcept { return y; }
};


/////////////////////////////////////
//vec3
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

    vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& other) noexcept = default;
	vec_type& operator=(const vec_type& other) noexcept = default;

    vec(vec_type&& other) noexcept = default;
    vec_type& operator=(vec_type&& other) noexcept = default;

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i];  return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other;  return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other;  return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other;  return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other;  return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other;  return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other;  return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other;  return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other;  return *this; }

    vec_type operator-() const { return vec_type(-x, -y, -z); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

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

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return z; }
    const_reference back() const noexcept { return z; }

    //custom
    vec2<value_type> xy() const noexcept { return vec2<value_type>(x,y); }
    vec2<value_type> yz() const noexcept { return vec2<value_type>(y,z); }
    vec2<value_type> xz() const noexcept { return vec2<value_type>(x,z); }
};



//////////////////////////////
//vec4
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

    vec() noexcept = default;
	~vec() noexcept = default;

	vec(const vec_type& other) noexcept = default;
	vec_type& operator=(const vec_type& other) noexcept = default;

    vec(vec_type&& other) noexcept = default;
    vec_type& operator=(vec_type&& other) noexcept = default;

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i];  return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i];  return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other;  return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other;  return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other;  return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other;  return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other;  return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other;  return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other;  return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other;  return *this; }

    vec_type operator-() const { return  vec_type(-x, -y, -z, -w); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

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

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data() + i; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return data() + i; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return w; }
    const_reference back() const noexcept { return w; }

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

//dim for std::min mostly
template<size_t dim, typename T> constexpr size_t vec<dim, T>::dim;
template<typename T> constexpr size_t vec<2, T>::dim;
template<typename T> constexpr size_t vec<3, T>::dim;
template<typename T> constexpr size_t vec<4, T>::dim;

//invalid specialization - therefore not specified, allow vec1?
template<typename T> class vec<0, T>;

//#include <nytl/refVec.hpp> to make those work!
template<size_t dim, typename T> class vec<dim, T&>;
template<typename T> class vec<2, T&>;
template<typename T> class vec<3, T&>;
template<typename T> class vec<4, T&>;

//operators/utility
#include <nytl/bits/vec.inl>

}
