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
#include <nytl/compFunc.hpp>
#include <nytl/misc.hpp>
#include <nytl/integer_sequence.hpp>

#include <iomanip>
#include <memory>
#include <cmath>
#include <tuple>
#include <cstddef>

namespace nytl
{

//typedefs
template<std::size_t rows, std::size_t cols, typename prec, typename Cond = typename std::enable_if<(rows >= 1) && (cols >= 1) && (!std::is_reference<prec>::value)>::type> class mat;
template<std::size_t n, class prec> using squareMat = mat<n, n, prec>;

template<class prec> using mat2 = squareMat<2,prec>;
template<class prec> using mat3 = squareMat<3,prec>;
template<class prec> using mat4 = squareMat<4,prec>;

template<class prec> using mat23 = mat<2,3,prec>;
template<class prec> using mat24 = mat<2,4,prec>;
template<class prec> using mat32 = mat<3,2,prec>;
template<class prec> using mat34 = mat<3,4,prec>;
template<class prec> using mat42 = mat<4,2,prec>;
template<class prec> using mat43 = mat<4,3,prec>;

typedef mat2<float> mat2f;
typedef mat2<unsigned int> mat2ui;
typedef mat2<int> mat2i;
typedef mat2<double> mat2d;
typedef mat2<char> mat2c;
typedef mat2<unsigned char> mat2uc;
typedef mat2<long> mat2l;
typedef mat2<unsigned long> mat2ul;
typedef mat2<short> mat2s;
typedef mat2<unsigned short> mat2us;

typedef mat3<float> mat3f;
typedef mat3<unsigned int> mat3ui;
typedef mat3<int> mat3i;
typedef mat3<double> mat3d;
typedef mat3<char> mat3c;
typedef mat3<unsigned char> mat3uc;
typedef mat3<long> mat3l;
typedef mat3<unsigned long> mat3ul;
typedef mat3<short> mat3s;
typedef mat3<unsigned short> mat3us;

typedef mat4<float> mat4f;
typedef mat4<unsigned int> mat4ui;
typedef mat4<int> mat4i;
typedef mat4<double> mat4d;
typedef mat4<char> mat4c;
typedef mat4<unsigned char> mat4uc;
typedef mat4<long> mat4l;
typedef mat4<unsigned long> mat4ul;
typedef mat4<short> mat4s;
typedef mat4<unsigned short> mat4us;


typedef mat23<float> mat23f;
typedef mat23<unsigned int> mat23ui;
typedef mat23<int> mat23i;
typedef mat23<double> mat23d;
typedef mat23<char> mat23c;
typedef mat23<unsigned char> mat23uc;
typedef mat23<long> mat23l;
typedef mat23<unsigned long> mat23ul;
typedef mat23<short> mat23s;
typedef mat23<unsigned short> mat23us;

typedef mat24<float> mat24f;
typedef mat24<unsigned int> mat24ui;
typedef mat24<int> mat24i;
typedef mat24<double> mat24d;
typedef mat24<char> mat24c;
typedef mat24<unsigned char> mat24uc;
typedef mat24<long> mat24l;
typedef mat24<unsigned long> mat24ul;
typedef mat24<short> mat24s;
typedef mat24<unsigned short> mat24us;

typedef mat32<float> mat32f;
typedef mat32<unsigned int> mat32ui;
typedef mat32<int> mat32i;
typedef mat32<double> mat32d;
typedef mat32<char> mat32c;
typedef mat32<unsigned char> mat32uc;
typedef mat32<long> mat32l;
typedef mat32<unsigned long> mat32ul;
typedef mat32<short> mat32s;
typedef mat32<unsigned short> mat32us;

typedef mat34<float> mat34f;
typedef mat34<unsigned int> mat34ui;
typedef mat34<int> mat34i;
typedef mat34<double> mat34d;
typedef mat34<char> mat34c;
typedef mat34<unsigned char> mat34uc;
typedef mat34<long> mat34l;
typedef mat34<unsigned long> mat34ul;
typedef mat34<short> mat34s;
typedef mat34<unsigned short> mat34us;

typedef mat42<float> mat42f;
typedef mat42<unsigned int> mat42ui;
typedef mat42<int> mat42i;
typedef mat42<double> mat42d;
typedef mat42<char> mat42c;
typedef mat42<unsigned char> mat42uc;
typedef mat42<long> mat42l;
typedef mat42<unsigned long> mat42ul;
typedef mat42<short> mat42s;
typedef mat42<unsigned short> mat42us;

typedef mat43<float> mat43f;
typedef mat43<unsigned int> mat43ui;
typedef mat43<int> mat43i;
typedef mat43<double> mat43d;
typedef mat43<char> mat43c;
typedef mat43<unsigned char> mat43uc;
typedef mat43<long> mat43l;
typedef mat43<unsigned long> mat43ul;
typedef mat43<short> mat43s;
typedef mat43<unsigned short> mat43us;

#include <nytl/bits/matmp.inl>

//mat class
template<std::size_t rows, std::size_t cols, typename prec, typename Cond>
class mat
{
public:
    using value_type = prec;
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

    using mat_type = mat<rows, cols, prec, Cond>;
    using rows_vec = vec<rows, prec>;
    using cols_vec = vec<cols, prec>;

    static constexpr bool is_squared = (rows == cols);
    static constexpr size_type mat_size = rows * cols;

public:
	vec<rows, vec<cols, prec>> data_;

public:
    template<typename... Args, typename = typename std::enable_if<std::is_convertible<std::tuple<Args...>, typename type_tuple<value_type, mat_size>::type>::value>::type>
    mat(Args&&... args) noexcept { constexpr detail::initMatData<make_index_sequence<rows * cols>> a{}; a(data_, std::make_tuple(args...)); }

	mat() noexcept = default;
	~mat() noexcept = default;

	mat(const mat_type& other) noexcept = default;
	mat(mat_type&& other) noexcept = default;

	mat_type& operator=(const mat_type& other) noexcept = default;
	mat_type& operator=(mat_type&& other) noexcept = default;

    template<typename... Args, typename = typename std::enable_if<std::is_convertible<std::tuple<Args...>, typename type_tuple<value_type, mat_size>::type>::value>::type>
    void init(Args&&... args) { constexpr detail::initMatData<make_index_sequence<rows * cols>> a{}; a(data_, std::make_tuple(args...)); }

    //access
	vec<cols, prec>& row(size_t i){ return data_[i]; }
	const vec<cols, prec>& row(size_t i) const { return data_[i]; }

	refVec<cols, prec> col(size_t i){ constexpr detail::makeRefVec<make_index_sequence<rows>> a{}; return a(data_, i); }
	vec<cols, prec> col(size_t i) const { vec<rows, prec> ret; for(size_t r(0); r < rows; r++)ret[r] = data_[r][i]; return ret; }

    //data
    pointer data(){ return (prec*)&data_; }
	const_pointer data() const { return (prec*)&data_; }
	std::unique_ptr<prec[]> copyData() const { constexpr detail::copyMatData<make_index_sequence<rows * cols>> a{}; return a(data_); }

    //math operators
    mat_type& operator +=(const mat<rows, cols, prec>& other){ data_ += other.data_; return *this; }
   	mat_type& operator -=(const mat<rows, cols, prec>& other){ data_ -= other.data_; return *this; }
    mat_type& operator *=(const mat<cols, rows, prec>& other)
        { auto od = data_; for(size_t r(0); r < rows; r++) for(size_t c(0); c < cols; c++) data_[r][c] = weight(od[r] * other.col(c)); return *this; }

    mat<rows, cols, prec>& operator *=(const prec& other){ for(auto& val : *this) val *= other; }

    //invert - todo
    bool invertable() const { return 0; }
    std::enable_if<is_squared, bool> invert() const { return invertable(); }

    //convert
    template<typename oprec> operator mat<rows, cols, oprec>() const
        { mat<rows, cols, oprec> ret; for(size_t r(0); r < rows; r++) for(size_t c(0); c < cols; c++) ret[r][c] = data_[r][c];  return ret; }
    template<std::size_t orows, std::size_t ocols, class oprec> operator mat<orows, ocols, oprec>() const
        { mat<orows, ocols, oprec> ret; for(size_t r(0); r < std::min(orows, rows); r++) for(size_t c(0); c < std::min(ocols, cols); c++) ret[r][c] = data_[r][c];  return ret; }

    //stl container
    constexpr size_type size() const { return mat_size; }
    constexpr bool empty() const { return 0; }

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

    vec<cols, prec>& operator[](size_t row){ return data_[row]; }
	const vec<cols, prec>& operator[](size_t row) const { return data_[row]; }

    vec<cols, prec>& at(size_t row){ if(row >= rows || row < 0)throw std::out_of_range("nyutil::mat::at: out of range"); return data_[row][col]; }
	const vec<cols, prec>& at(size_t row) const { if(row >= rows || row < 0)throw std::out_of_range("nyutil::mat::at: out of range"); return data_[row][col]; }

	prec& at(size_t row, size_t col){ if(row >= rows || row < 0)throw std::out_of_range("nyutil::mat::at: out of range"); return data_[row]; }
	const prec& at(size_t row, size_t col) const { if(row >= rows || row < 0)throw std::out_of_range("nyutil::mat::at: out of range"); return data_[row]; }

    reference front() noexcept { return data_[0][0]; }
    const_reference front() const noexcept { return data_[0][0]; }

    reference back() noexcept { return data_[rows - 1][cols - 1]; }
    const_reference back() const noexcept { return data_[rows - 1][cols - 1]; }
};

//operators and utility functions
#include <nytl/bits/mat.inl>

} //nyutil
