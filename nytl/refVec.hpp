/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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
///\brief Makes the Vec specialization for reference types work.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/bits/refVecIterator.inl>

namespace nytl
{

template<size_t dim, typename T> using RefVec = Vec<dim, T&>;

template<typename T> using RefVec2 = Vec<2, T&>;
template<typename T> using RefVec3 = Vec<3, T&>;
template<typename T> using RefVec4 = Vec<4, T&>;

typedef RefVec2<float> RefVec2f;
typedef RefVec2<int> RefVec2i;
typedef RefVec2<unsigned int> RefVec2ui;
typedef RefVec2<double> RefVec2d;
typedef RefVec2<char> RefVec2c;
typedef RefVec2<unsigned char> RefVec2uc;
typedef RefVec2<long> RefVec2l;
typedef RefVec2<unsigned long> RefVec2ul;

typedef RefVec3<float> RefVec3f;
typedef RefVec3<int> RefVec3i;
typedef RefVec3<unsigned int> RefVec3ui;
typedef RefVec3<double> RefVec3d;
typedef RefVec3<char> RefVec3c;
typedef RefVec3<unsigned char> RefVec3uc;
typedef RefVec3<long> RefVec3l;
typedef RefVec3<unsigned long> RefVec3ul;

typedef RefVec4<float> RefVec4f;
typedef RefVec4<int> RefVec4i;
typedef RefVec4<unsigned int> RefVec4ui;
typedef RefVec4<double> RefVec4d;
typedef RefVec4<char> RefVec4c;
typedef RefVec4<unsigned char> RefVec4uc;
typedef RefVec4<long> RefVec4l;
typedef RefVec4<unsigned long> RefVec4ul;


///\ingroup math
///Vec specialization for reference types.
template<size_t dimension, typename T> 
class Vec<dimension, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = dimension;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = RefVecIterator<dim, value_type>;
    using const_iterator = constRefVecIterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using ref_VecType = Vec<dimension, reference>;
    using VecType = Vec<dimension, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    pointer data_[dim];

public:
    template<typename... Args, typename = typename 
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>, 
				typename type_tuple<value_type, dim>::type
			>::value
		>::type>
    Vec(Args&&... args) : data_{&args...} {}
    ~Vec() noexcept = default;

    Vec(const ref_VecType& other) noexcept = default;
    Vec(const VecType& other) noexcept 
		{ for(size_t i(0); i < dim; ++i) data_[i] = &other[i]; }

    ref_VecType& operator=(const VecType& other) noexcept 
		{ for(size_t i(0); i < dim; ++i) *data_[i] = other[i]; return *this; }
    ref_VecType& operator=(const ref_VecType& other) noexcept //needed?
		{ for(size_t i(0); i < dim; ++i) *data_[i] = other[i]; return *this; }

    //operator
    template <size_t odim, typename ot> ref_VecType& operator +=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator -=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator *=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator /=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator %=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator |=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator ^=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator &=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator >>=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator <<=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> ref_VecType& operator +=(ot other)
		{ for(auto& val : *this) val += other; return *this; }
    template<typename ot> ref_VecType& operator -=(ot other)
		{ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> ref_VecType& operator *=(ot other)
		{ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> ref_VecType& operator /=(ot other)
		{ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> ref_VecType& operator %=(ot other)
		{ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> ref_VecType& operator |=(ot other)
		{ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> ref_VecType& operator ^=(ot other)
		{ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> ref_VecType& operator &=(ot other)
		{ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> ref_VecType& operator >>=(ot other)
		{ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> ref_VecType& operator <<=(ot other)
		{ for(auto& val : *this) val <<= other; return *this; }

    VecType operator-() const 
		{ VecType ret(*this); for(size_t i(0); i < dim; i++) ret[i] -= (*this)[i]; return ret; }

    template <size_t odim, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<odim, ot>() const 
	{ 
		Vec<odim, ot> ret; 
		ret.fill(ot()); 
		for(size_t i(0); i < std::min(odim, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	//stl
    const_pointer data() const noexcept { return data_; }
    pointer data() noexcept { return data_; }

    void fill(const value_type& val) { for(auto& v : data_) *v = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return *data_[i]; }
    const_reference operator[](size_type i) const { return *data_[i]; }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return *data_[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return *data_[i]; }

    reference front() noexcept { return *data_[0]; }
    const_reference front() const noexcept { return *data_[0]; }

    reference back() noexcept { return *data_[dim - 1]; }
    const_reference back() const noexcept { return *data_[dim - 1]; }

	void swap(VecType& other)
		{ for(std::size_t i(0); i < size(); ++i) std::swap(*data_[i], other[i]); }

	friend void swap(ref_VecType& a, ref_VecType& b) { a.swap(b); }
};

template<typename T> class Vec<2, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 2;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = RefVecIterator<dim, value_type>;
    using const_iterator = constRefVecIterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using ref_VecType = Vec<dim, reference>;
    using VecType = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;

public:
    Vec(reference a, reference b) : x(a), y(b) {}
    ~Vec() noexcept = default;

    Vec(const ref_VecType& other) noexcept = default;

    Vec(VecType& other) noexcept : x(other.x), y(other.y) {}
    ref_VecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; return *this; }
    ref_VecType& operator=(const ref_VecType& other) noexcept 
		{ x = other.x; y = other.y; return *this; }

    //operator
    template <size_t odim, typename ot> ref_VecType& operator +=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator -=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator *=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator /=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator %=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator |=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator ^=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator &=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator >>=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator <<=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> ref_VecType& operator +=(ot other)
		{ for(auto& val : *this) val += other; return *this; }
    template<typename ot> ref_VecType& operator -=(ot other)
		{ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> ref_VecType& operator *=(ot other)
		{ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> ref_VecType& operator /=(ot other)
		{ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> ref_VecType& operator %=(ot other)
		{ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> ref_VecType& operator |=(ot other)
		{ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> ref_VecType& operator ^=(ot other)
		{ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> ref_VecType& operator &=(ot other)
		{ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> ref_VecType& operator >>=(ot other)
		{ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> ref_VecType& operator <<=(ot other)
		{ for(auto& val : *this) val <<= other; return *this; }

    VecType operator-() const { return VecType(x, y); }

    template <size_t odim, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<odim, ot>() const 
	{ 
		Vec<odim, ot> ret; 
		ret.fill(ot());
	   	for(size_t i(0); i < std::min(odim, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ if(i == 0) return x; return y; }
    const_reference operator[](size_type i) const { if(i == 0) return x; return y; }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return y; }
    const_reference back() const noexcept { return y; }

	void swap(VecType& other){ std::swap(x, other.x); std::swap(y, other.y); }

	friend void swap(ref_VecType& a, ref_VecType& b) { a.swap(b); }
};

template<typename T> class Vec<3, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 3;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = RefVecIterator<dim, value_type>;
    using const_iterator = constRefVecIterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using ref_VecType = Vec<dim, reference>;
    using VecType = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;
    reference z;

public:
    Vec(reference a, reference b, reference c) noexcept : x(a), y(b), z(c) {}
    ~Vec() noexcept = default;

    Vec(const ref_VecType& other) noexcept = default;

    Vec(const VecType& other) noexcept : x(other.x), y(other.y), z(other.z) {}
    ref_VecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; return *this; } 
    ref_VecType& operator=(const ref_VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; return *this; }

    //operator
    template <size_t odim, typename ot> ref_VecType& operator +=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator -=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator *=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator /=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator %=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator |=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator ^=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator &=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator >>=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator <<=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> ref_VecType& operator +=(ot other)
		{ for(auto& val : *this) val += other; return *this; }
    template<typename ot> ref_VecType& operator -=(ot other)
		{ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> ref_VecType& operator *=(ot other)
		{ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> ref_VecType& operator /=(ot other)
		{ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> ref_VecType& operator %=(ot other)
		{ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> ref_VecType& operator |=(ot other)
		{ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> ref_VecType& operator ^=(ot other)
		{ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> ref_VecType& operator &=(ot other)
		{ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> ref_VecType& operator >>=(ot other)
		{ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> ref_VecType& operator <<=(ot other)
		{ for(auto& val : *this) val <<= other; return *this; }

    VecType operator-() const { return VecType(x, y, z); }

    template <size_t odim, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<odim, ot>() const 
	{ 
		Vec<odim, ot> ret;
	   	ret.fill(ot());
	   	for(size_t i(0); i < std::min(odim, dim); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i)
		{ if(i == 0) return x; if(i == 1) return y; return z; }
    const_reference operator[](size_type i) const 
		{ if(i == 0) return x; if(i == 1) return y; return z; }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return z; }
    const_reference back() const noexcept { return z; }

	void swap(VecType& other)
		{ std::swap(x, other.x); std::swap(y, other.y); std::swap(z, other.z); }

	friend void swap(ref_VecType& a, ref_VecType& b) { a.swap(b); }

    //custom
    Vec2<reference> xy() const noexcept { return Vec2<reference>(x,y); }
    Vec2<reference> yz() const noexcept { return Vec2<reference>(y,z); }
    Vec2<reference> xz() const noexcept { return Vec2<reference>(x,z); }
};

template<typename T> class Vec<4, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 4;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = RefVecIterator<dim, value_type>;
    using const_iterator = constRefVecIterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using ref_VecType = Vec<dim, reference>;
    using VecType = Vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;
    reference z;
    reference w;

public:
    Vec(reference a, reference b, reference c, reference d) noexcept : x(a), y(b), z(c), w(d) {}
    ~Vec() noexcept = default;

    Vec(const ref_VecType& other) noexcept = default;

    Vec(const VecType& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {} 
    ref_VecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
    ref_VecType& operator=(const ref_VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; w = other.w; return *this; }

    //operator
    template <size_t odim, typename ot> ref_VecType& operator +=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator -=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator *=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator /=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator %=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator |=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator ^=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator &=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator >>=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> ref_VecType& operator <<=(const Vec<odim, ot>& other)
		{ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> ref_VecType& operator +=(ot other)
		{ for(auto& val : *this) val += other; return *this; }
    template<typename ot> ref_VecType& operator -=(ot other)
		{ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> ref_VecType& operator *=(ot other)
		{ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> ref_VecType& operator /=(ot other)
		{ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> ref_VecType& operator %=(ot other)
		{ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> ref_VecType& operator |=(ot other)
		{ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> ref_VecType& operator ^=(ot other)
		{ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> ref_VecType& operator &=(ot other)
		{ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> ref_VecType& operator >>=(ot other)
		{ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> ref_VecType& operator <<=(ot other)
		{ for(auto& val : *this) val <<= other; return *this; }

    VecType operator-() const { return VecType(x, y, z); }

    template <size_t odim, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<odim, ot>() const
	{ 
		Vec<odim, ot> ret;
	   	ret.fill(ot());
	   	for(size_t i(0); i < std::min(odim, dim); i++)
		   	ret[i] = (*this)[i]; 
		return ret; 
	}

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; w = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i)
		{ if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }
    const_reference operator[](size_type i) const 
		{ if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }

    reference at(size_type i){ if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) 
		throw std::out_of_range("nytl::Vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return w; }
    const_reference back() const noexcept { return w; }

	void swap(VecType& other)
		{ using std::swap; swap(x, other.x); swap(y, other.y); swap(z, other.z); swap(w, other.w);}

	friend void swap(ref_VecType& a, ref_VecType& b){ a.swap(b); }

    //custom
    Vec2<reference> xy() const noexcept { return Vec2<reference>(x,y); }
    Vec2<reference> xz() const noexcept { return Vec2<reference>(x,z); }
    Vec2<reference> xw() const noexcept { return Vec2<reference>(x,w); }
    Vec2<reference> yz() const noexcept { return Vec2<reference>(y,z); }
    Vec2<reference> yw() const noexcept { return Vec2<reference>(y,w); }
    Vec2<reference> zw() const noexcept { return Vec2<reference>(z,w); }

    Vec3<reference> xyz() const noexcept { return Vec3<reference>(x,y,z); }
    Vec3<reference> xyw() const noexcept { return Vec3<reference>(x,y,w); }
    Vec3<reference> xzw() const noexcept { return Vec3<reference>(x,z,w); }
    Vec3<reference> yzw() const noexcept { return Vec3<reference>(y,z,w); }
};

template<size_t dim, typename T> constexpr size_t Vec<dim, T&>::dim;
template<typename T> constexpr size_t Vec<2, T&>::dim;
template<typename T> constexpr size_t Vec<3, T&>::dim;
template<typename T> constexpr size_t Vec<4, T&>::dim;

}

