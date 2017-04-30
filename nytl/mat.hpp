// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the nytl::Mat Matrix template class.

#pragma once

#ifndef NYTL_INCLUDE_MAT
#define NYTL_INCLUDE_MAT

#include <nytl/fwd/mat.hpp> // nytl::Mat forward declaration
#include <nytl/vec.hpp> // nytl::Vec
#include <nytl/vecOps.hpp> // nytl::vec::dot
#include <nytl/matOps.hpp> // nytl::mat::row

namespace nytl {

/// \brief A Matrix that with 'R' rows and 'C' columns over type 'T' on the stack.
/// Satisfies the Matrix concepts used by nytl/matOps.
/// \tparam T The value type of the matrix.
/// \tparam R The rows of the matrix.
/// \tparam C The columns of the matrix.
template<size_t R, size_t C, typename T>
struct Mat {
public:
	Vec<R, Vec<C, T>> data_;

public:
	using Value = T;
	using Size = size_t;
	using RowVec = Vec<C, T>;
	using ColVec = Vec<R, T>;

	/// Rebinds the matrix implementation to another value type.
	template<typename OT> using Rebind = Mat<R, C, OT>;

	/// Static sized matrix implementation
	static constexpr auto staticSized = true;

	/// Creates a new matrix with the given size
	template<Size OR, Size OC>
	static constexpr Mat<OR, OC, T> create() { return {}; }

	/// The (static/fixed) dimensions of the matrix
	static constexpr auto rows() { return R; }
	static constexpr auto cols() { return C; }

public:
	/// Returns the ith row of the matrix.
	constexpr RowVec& operator[](Size i){ return data_[i]; }
	constexpr const RowVec& operator[](Size i) const { return data_[i]; }

	/// Returns the ith row of the matrix.
	/// If these exceeds the size of the matrix, throws std::out_of_range.
	constexpr RowVec& at(Size r) { checkRow(r); return data_[r]; }
	constexpr const RowVec& at(Size r) const { checkRow(r); return data_[r]; }

	/// Returns the value at position (r, c).
	/// If this position exceeds the size of the matrix, throws std::out_of_range.
	constexpr T& at(Size r, Size c) { return at(r).at(c); }
	constexpr const T& at(Size r, Size c) const { return at(r).at(c); }

	/// Utility function that throws std::out_of_range if the matrix does not have
	/// the given row.
	constexpr void checkRow(Size r) { if(r >= rows()) throw std::out_of_range("nytl::Mat::at"); }
};

// Dynamic-sized specialization of the Mat type
// See/include nytl/dynMat.hpp
template<typename T> struct Mat<0, 0, T>;

// Invalid instanciations
template<size_t R, typename T> struct Mat<R, 0, T>;
template<size_t C, typename T> struct Mat<0, C, T>;


// - implementation/operators -
template<typename T1, typename T2, size_t R1, size_t C1, size_t R2, size_t C2>
constexpr auto operator*(const Mat<R1, C1, T1>& a, const Mat<R2, C2, T2>& b)
{
	mat::detail::assertMultDimensions(a, b);
	constexpr auto dyn = !(Mat<R1, C1, T1>::staticSized && Mat<R2, C2, T2>::staticSized);

	auto ret = Mat<dyn ? 0 : R1, dyn ? 0 : C2, decltype(a[0][0] * b[0][0] + a[0][0] * b[0][0])> {};
	if constexpr(dyn) ret.resize(a.rows(), b.cols());

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret[r][c] = vec::dot(mat::row(a, r), mat::col(b, c));

	return ret;
}

template<typename T1, typename T2, size_t R, size_t C, size_t VD>
constexpr auto operator*(const Mat<R, C, T1>& a, const Vec<VD, T2>& b)
{
	vec::detail::assertSameDimensions(a[0], b);
	constexpr auto dyn = !Mat<R, C, T1>::staticSized;

	auto ret = Vec<dyn ? 0 : R, decltype(a[0][0] * b[0] + a[0][0] * b[0])> {};
	if constexpr(!decltype(ret)::staticSized) ret.resize(a.rows());

	for(auto r = 0u; r < ret.size(); ++r)
		ret[r] = vec::dot(mat::row(a, r), b);

	return ret;
}

template<typename F, typename T, size_t R, size_t C>
constexpr auto operator*(const F& f, const Mat<R, C, T>& a)
	-> decltype(mat::detail::createMatrix<decltype(f * a[0][0])>(a))
{
	auto ret = mat::detail::createMatrix<decltype(f * a[0][0])>(a);

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret[r][c] = f * a[r][c];

	return ret;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator+(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	mat::detail::assertSameDimensions(a, b);
	auto ret = mat::detail::createMatrix<decltype(a[0][0] + b[0][0])>(a);

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret[r][c] = a[r][c] + b[r][c];

	return ret;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator-(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	mat::detail::assertSameDimensions(a, b);
	auto ret = mat::detail::createMatrix<decltype(a[0][0] - b[0][0])>(a);

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret[r][c] = a[r][c] - b[r][c];

	return ret;
}

template<typename T, size_t R, size_t C>
constexpr auto operator-(const Mat<R, C, T>& a)
{
	auto ret = mat::detail::createMatrix<decltype(-a[0][0])>(a);

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret[r][c] = -a[r][c];

	return ret;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator==(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	mat::detail::assertSameDimensions(a, b);

	for(auto r = 0u; r < a.rows(); ++r)
		for(auto c = 0u; c < a.cols(); ++c)
			if(a[r][c] != b[r][c])
				return false;

	return true;
}

template<typename T1, typename T2, size_t R, size_t C>
constexpr auto operator!=(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	return !(a == b);
}

template<typename T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Mat<R, C, T>& a)
{
	return mat::print(os, a);
}

} // namespace nytl

#endif // header guard
