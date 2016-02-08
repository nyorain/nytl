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
///\brief Makes the dynamic-sized Vec specialization work

#pragma once

#include <nytl/vec.hpp>
#include <vector>

//notes:
//for DynVec<bool>, the Vector<bool> implementation is used which can lead to problems.
//extra specialization for it? better use different container for internal DynVec storage?
//use valarray for implemenation instead?

namespace nytl
{

template<typename T> using DynVec = Vec<dynamicSize, T>;

using DynVecf = DynVec<float>;
using DynVeci = DynVec<int>;
using DynVecui = DynVec<unsigned int>;
using DynVecc = DynVec<char>;
using DynVecuc = DynVec<unsigned char>;
using DynVecl = DynVec<long>;
using DynVecul = DynVec<unsigned long>;
using DynVecd = DynVec<double>;
using DynVecb = DynVec<bool>;

///\ingroup math
///Vec specialization for a dynamic-sized Vec.
template<typename T>
class Vec<dynamicSize, T>
{
public:
    using value_type = T;
	using Vector_type = std::vector<T>;
    constexpr static size_t dim = dynamicSize;

    using reference = typename Vector_type::reference;
    using const_reference = typename Vector_type::const_reference;
    using pointer = typename Vector_type::pointer;
    using const_pointer = typename Vector_type::const_pointer;
    using iterator = typename Vector_type::iterator;
    using const_iterator = typename Vector_type::const_iterator;
    using reverse_iterator = typename Vector_type::reverse_iterator;
    using const_reverse_iterator = typename Vector_type::const_reverse_iterator;
    using difference_type = typename Vector_type::difference_type;
    using size_type = std::size_t;

    using VecType = Vec;

public:
    size_t max_size() const noexcept { return data_.max_size(); }
    size_t size() const noexcept { return data_.size(); }
    bool empty() const noexcept { return size() == 0; }

public:
	Vector_type data_ {};

public:
	Vec(std::initializer_list<T> init) : data_(init) {}
	explicit Vec(size_type count) : data_(count) {}
	explicit Vec(size_type count, const T& val) : data_(count, val) {}

	template<std::size_t OD, typename OT>
	Vec(const Vec<OD, OT>& other) : data_(other.size())
	{
		for(std::size_t i(0); i < other.size(); ++i) data_[i] = other[i];
	}

    Vec() = default;
    ~Vec() noexcept = default;

    Vec(const VecType& other) = default;
    VecType& operator=(const VecType& other) = default;

    Vec(VecType&& other) noexcept = default;
    VecType& operator=(VecType&& other) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), size()); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename ot> VecType& operator +=(const ot& lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename ot> VecType& operator -=(const ot& lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename ot> VecType& operator *=(const ot& lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename ot> VecType& operator /=(const ot& lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename ot> VecType& operator %=(const ot& lhs){ 
		for(auto& val : *this) val %= lhs; return *this; }
    template<typename ot> VecType& operator |=(const ot& lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename ot> VecType& operator ^=(const ot& lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename ot> VecType& operator &=(const ot& lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename ot> VecType& operator >>=(const ot& lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename ot> VecType& operator <<=(const ot& lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    VecType operator-() const 
		{ VecType ret{}; for(size_t i(0); i < size(); i++) ret[i] -= (*this)[i]; return ret; }

    template <std::size_t OD, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<OD, ot>() const 
		{ 
			Vec<OD, ot> ret(size()); 
			ret.fill(ot()); 
			for(size_t i(0); i < min(ret.size(), size()); i++) 
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

	//dynamic stuff, todo. Just forward all Vec member functions here??
	void clear(){ data_.clear(); }
	void add(const T& val){ data_.push_back(val); }
	void resize(size_type size){ data_.resize(size); }
	void resize(size_type size, const T& val){ data_.resize(size, val); }
	void reserve(size_type size){ data_.reserve(size); }

    //stl norm stuff
    const_pointer data() const noexcept { return data_.data(); }
    pointer data() noexcept { return data_.data(); }

    void fill(const value_type& val) { for(auto& v : data_) v = val; }

    iterator begin() noexcept { return data_.begin(); }
	const_iterator begin() const noexcept { return data_.cbegin(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }

    iterator end() noexcept { return data_.end(); }
    const_iterator end() const noexcept { return data_.cend(); }
    const_iterator cend() const noexcept { return data_.cend(); }

    reverse_iterator rbegin() noexcept { return data_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return data_.crbegin(); }
    const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }

    reverse_iterator rend() noexcept { return data_.rend(); }
    const_reverse_iterator rend() const noexcept { return data_.crend(); }
    const_reverse_iterator crend() const noexcept { return data_.crend(); }

    reference operator[](size_type i){ return data_[i]; }
    const_reference operator[](size_type i) const { return data_[i]; }

    reference at(size_type i){ return data_.at(i); }
    const_reference at(size_type i) const { return data_.at(i); }

    reference front() noexcept { return data_.front(); }
    const_reference front() const noexcept { return data_.front(); }

    reference back() noexcept { return data_.back(); }
    const_reference back() const noexcept { return data_.back(); }

	void swap(VecType& other){ data_.swap(other.data_); }
};

}
