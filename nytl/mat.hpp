// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// Defines the nytl::Mat Matrix template class.

#pragma once

#ifndef NYTL_INCLUDE_MAT
#define NYTL_INCLUDE_MAT

#include <nytl/fwd/mat.hpp> // nytl::Mat forward declaration
#include <nytl/vec.hpp> // nytl::Vec
#include <nytl/vecOps.hpp> // nytl::dot

namespace nytl {

/// A Matrix with 'R' rows and 'C' columns over type 'T' on the stack.
/// Data is stored (and accessed) with row-major semantics.
/// - T: The value type of the matrix.
/// - R: The rows of the matrix.
/// - C: The columns of the matrix.
template<size_t R, size_t C, typename T>
struct Mat {
	/// The (static/fixed) dimensions of the matrix
	static constexpr auto rows() { return R; }
	static constexpr auto cols() { return C; }

public:
	/// Returns the ith row of the matrix.
	/// Since this is of type nytl::Vec, mat[i][j] works.
	constexpr auto& operator[](size_t i){ return rows_[i]; }
	constexpr const auto& operator[](size_t i) const { return rows_[i]; }

	/// Returns the ith row of the matrix.
	/// If these exceeds the size of the matrix, throws std::out_of_range.
	constexpr auto& at(size_t r) { checkRow(r); return rows_[r]; }
	constexpr const auto& at(size_t r) const { checkRow(r); return rows_[r]; }

	/// Returns the value at position (r, c).
	/// If this position exceeds the size of the matrix, throws std::out_of_range.
	constexpr auto& at(size_t r, size_t c) { return at(r).at(c); }
	constexpr const auto& at(size_t r, size_t c) const { return at(r).at(c); }

	/// Explicitly casts the Mat to another Mat that may have
	/// a different precision or dimensions. Will default construct
	/// any values that cannot be filled (e.g. mat33 -> mat44) or leave
	/// out the last values when the size of vector is shrinked (e.g.
	/// {11, 12, 13, 21, 22, 23} -> {11, 12, 21, 22}).
	template<size_t OR, size_t OC, typename OT>
	constexpr explicit operator Mat<OR, OC, OT>() const;

	/// Utility function that throws std::out_of_range if the matrix does not have
	/// the given row.
	constexpr void checkRow(size_t r) {
		if(r >= rows())
			throw std::out_of_range("nytl::Mat::at");
	}

public:
	std::array<Vec<C, T>, R> rows_;
};

// - implementation/operators -
template<size_t R, size_t C, typename T>
template<size_t OR, size_t OC, typename OT>
constexpr Mat<R, C, T>::operator Mat<OR, OC, OT>() const {
	Mat<OR, OC, OT> ret {};
	for(auto r = 0u; r < std::min(R, OR); ++r)
		for(auto c = 0u; c < std::min(C, OC); ++c)
			ret[r][c] = (*this)[r][c];
	return ret;
}

// mat * mat
template<typename T1, typename T2, size_t R, size_t M, size_t C>
constexpr auto operator*(const Mat<R, M, T1>& a, const Mat<M, C, T2>& b) {
	Mat<R, C, decltype(a[0][0] * b[0][0] + a[0][0] * b[0][0])> ret {};
	for(auto r = 0u; r < R; ++r) // ret: rows
		for(auto c = 0u; c < C; ++c) // ret: cols
			for(auto i = 0u; i < M; ++i) // row + col dot
				ret[r][c] += a[r][i] * b[i][c];
	return ret;
}

// mat * vec
template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator*(const Mat<R, C, T1>& a, const Vec<C, T2>& b) {
	Vec<R, decltype(a[0][0] * b[0] + a[0][0] * b[0])> ret {};
	for(auto r = 0u; r < R; ++r)
		ret[r] = dot(a[r], b);
	return ret;
}

// mat *= mat (quadratic)
template<typename T1, typename T2, size_t D>
constexpr auto& operator*=(Mat<D, D, T1>& a, const Mat<D, D, T2>& b) {
	auto tmp = a; // needed since we write to a
	a = {};
	for(auto r = 0u; r < D; ++r) // ret: rows
		for(auto c = 0u; c < D; ++c) // ret: cols
			for(auto i = 0u; i < D; ++i) // row + col dot
				a[r][c] += tmp[r][i] * b[i][c];
	return a;
}

// fac * mat
template<typename F, typename T, size_t R, size_t C>
constexpr auto operator*(const F& f, const Mat<R, C, T>& a) {
	Mat<R, C, decltype(f * a[0][0])> ret {};
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = f * a[r][c];
	return ret;
}

// mat *= fac
template<typename F, typename T, size_t R, size_t C>
constexpr auto& operator*=(Mat<R, C, T>& a, const F& f) {
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			a[r][c] *= f;
	return a;
}

// mat + mat
template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator+(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	Mat<R, C, decltype(a[0][0] + b[0][0])> ret {};
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = a[r][c] + b[r][c];
	return ret;
}

// mat += mat
template<typename T1, typename T2, size_t R, size_t C>
constexpr auto& operator+=(Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			a[r][c] += b[r][c];
	return a;
}

// mat - mat
template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator-(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	Mat<R, C, decltype(a[0][0] - b[0][0])> ret {};
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = a[r][c] - b[r][c];
	return ret;
}

// mat -= mat
template<typename T1, typename T2, size_t R, size_t C>
constexpr auto& operator-=(Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			a[r][c] -= b[r][c];
	return a;
}

// -mat
template<typename T, size_t R, size_t C>
constexpr auto operator-(Mat<R, C, T> a) {
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			a[r][c] = -a[r][c];
	return a;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator==(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	for(auto i = 0u; i < R; ++i)
		if(a.rows_[i] != b.rows_[i])
			return false;
	return true;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator!=(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b) {
	return !(a == b);
}

} // namespace nytl

#undef nytl_assure
#endif // header guard
