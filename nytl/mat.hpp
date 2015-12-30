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
#include <nytl/refVec.hpp>
#include <nytl/integer_sequence.hpp>

#include <iomanip>
#include <memory>
#include <cmath>
#include <tuple>
#include <cstddef>

namespace nytl
{

template<std::size_t R, std::size_t C, typename P, typename = typename 
	std::enable_if<
		(R >= 1) && 
		(C >= 1) && 
		(!std::is_reference<P>::value)
	>::type>
class mat;

#include <nytl/bits/matmp.inl>
#include <nytl/bits/matypes.inl>

///\brief Templated matrix class with \c R rows and \c C columns of precision \c P.
template<std::size_t R, std::size_t C, typename P, typename Cond>
class mat
{
public:
    using value_type = P;
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

    using mat_type = mat<R, C, P, Cond>;
    using rows_vec = vec<R, P>;
    using cols_vec = vec<C, P>;

    static constexpr bool is_squared = (R == C);
    static constexpr size_type mat_size = R * C;

public:
	vec<R, vec<C, P>> data_;

public:
    template<typename... Args, typename = typename 
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>, 
				typename type_tuple<value_type, mat_size>::type
			>::value>
		::type>
    mat(Args&&... args) noexcept 
		{ detail::initMatData<R * C>::call(data_, std::make_tuple(args...)); }

	mat(const P& val) noexcept
		{ detail::initMatData<R * C>::call(data_, val); }

	mat() noexcept = default;
	~mat() noexcept = default;

	mat(const mat_type& other) noexcept = default;
	mat(mat_type&& other) noexcept = default;

	mat_type& operator=(const mat_type& other) noexcept = default;
	mat_type& operator=(mat_type&& other) noexcept = default;

	///Initialized the matrix with the given values
    template<typename... Args, typename = typename 
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>, 
				typename type_tuple<value_type, mat_size>::type
			>::value>
		::type>
    void init(Args&&... args) 
		{ detail::initMatData<R * C>::call(data_, std::make_tuple(args...)); }

    ///Returns a reference of a certain row of the matrix.
	///If captured with a reference (and got by a non-const object) the actual values of the
	///matrix can be changed with it.
	vec<C, P>& row(size_t i){ return data_[i]; }

	///Returns a conct-reference of a certain row of the matrix.
	const vec<C, P>& row(size_t i) const { return data_[i]; }

	///Returns a reference vec of a certain column of the matrix.
	///Must be captured by a refVec (not possible with vec&) to be able to change the actual
	///values of the matrix object.
	refVec<R, P> col(size_t i){ return detail::makeRowRefVec<R>::call(data_, i); }

	///Returns a vec (just the values, no references) of a certain row.
	vec<R, P> col(size_t i) const { return detail::makeRowVec<R>::call(data_, i); }

    ///Returns a plain pointer to the data of the matrix.
    pointer data(){ return (pointer) &data_.data(); }

	///Returns a const plain pointer to the data of the matrix.
	const_pointer data() const { return (pointer) &data_.data(); }

	///Copys the data of the matrix as plain unique ptr on the heap.
	std::unique_ptr<P[]> copyData() const { return detail::copyMatData<R * C>::call(data_); }

	///Returns a std::tuple filled with the components of the matrix
	type_tuple_t<P, mat_size> asTuple() const { return detail::matTuple<R * C>::call(data_); }

    //math
    mat_type& operator +=(const mat<R, C, P>& other){ data_ += other.data_; return *this; }
   	mat_type& operator -=(const mat<R, C, P>& other){ data_ -= other.data_; return *this; }
    mat_type& operator *=(const mat<C, R, P>& other)
    { 
		auto od = data_; 
		for(size_t r(0); r < R; r++) 
			for(size_t c(0); c < C; c++) 
				data_[r][c] = sum(od[r] * other.col(c)); 
		return *this; 
	}
    mat<R, C, P>& operator *=(const P& other){ for(auto& val : *this) val *= other; }

    //invert
    bool invertable() const { return 0; }
    std::enable_if<is_squared, bool> invert() const { return invertable(); }

    //convert
    template<std::size_t OR, std::size_t OC, class OP> operator mat<OR, OC, OP>() const;

    //stl container
    constexpr size_type size() const { return mat_size; }
    constexpr bool empty() const { return size() == 0; }

    void fill(const value_type& val) { for(auto& r : data_)for(auto& c : r) c = val; }

    iterator begin() noexcept { return &data_[0][0]; }
    const_iterator begin() const noexcept { return &data_[0][0]; }
    const_iterator cbegin() const noexcept { return &data_[0][0]; }

    iterator end() noexcept { return begin() + mat_size; }
    const_iterator end() const noexcept { return begin() + mat_size; }
    const_iterator cend() const noexcept { return begin() + mat_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    vec<C, P>& operator[](size_t row){ return data_[row]; }
	const vec<C, P>& operator[](size_t row) const { return data_[row]; }

    vec<C, P>& at(size_t row){ if(row >= R || row < 0)
		throw std::out_of_range("nytl::mat::at: out of range"); return data_[row][col]; }
	const vec<C, P>& at(size_t row) const { if(row >= R || row < 0)
		throw std::out_of_range("nytl::mat::at: out of range"); return data_[row][col]; }

	P& at(size_t row, size_t col){ if(row >= R || row < 0)
		throw std::out_of_range("nytl::mat::at: out of range"); return data_[row]; }
	const P& at(size_t row, size_t col) const { if(row >= R || row < 0)
		throw std::out_of_range("nytl::mat::at: out of range"); return data_[row]; }

    reference front() noexcept { return data_[0][0]; }
    const_reference front() const noexcept { return data_[0][0]; }

    reference back() noexcept { return data_[R - 1][C - 1]; }
    const_reference back() const noexcept { return data_[R - 1][C - 1]; }
};

//operators and utility functions
#include <nytl/bits/mat.inl>

} //nytl

