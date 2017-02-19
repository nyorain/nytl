// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the nytl::Mat Matrix template class.

#pragma once

#ifndef NYTL_INCLUDE_MAT_HPP
#define NYTL_INCLUDE_MAT_HPP

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
template<std::size_t R, std::size_t C, typename T>
struct Mat {
public:
	Vec<R, Vec<C, T>> data_;

public:
	using Value = T;
	using Size = std::size_t;
	using RowVec = Vec<C, T>;
	using ColVec = Vec<R, T>;

	constexpr static auto rowDim = R;
	constexpr static auto colDim = C;

	template<Size OR, Size OC, typename OT> using Rebind = Mat<OR, OC, OT>;
	constexpr static Mat create(Size r, Size c)
	{
		Mat ret {};
		ret.data_ = Vec<R, Vec<C, T>>::create(r);
		for(auto& row : ret.data_) row = Vec<C, T>::create(c);
		return ret;
	}

	constexpr auto rows() const { return data_.size(); }
	constexpr auto cols() const { return data_[0].size(); }

public:
	constexpr RowVec& operator[](Size i){ return data_[i]; }
	constexpr const RowVec& operator[](Size i) const { return data_[i]; }

	constexpr RowVec& at(Size r)
		{ if(r >= rows()) throw std::out_of_range("nytl::Mat::at"); return data_[r]; }
	constexpr const RowVec& at(Size r) const
		{ if(r >= rows()) throw std::out_of_range("nytl::Mat::at"); return data_[r]; }

	constexpr T& at(Size r, Size c) { return at(r).at(c); }
	constexpr const T& at(Size r, Size c) const { return at(r).at(c); }
};

// - operators -
template<typename T1, typename T2, std::size_t R, std::size_t M, std::size_t C>
constexpr auto operator*(const Mat<R, M, T1>& a, const Mat<M, C, T2>& b)
{
	using RetType = decltype(a[0][0] * b[0][0] + a[0][0] * b[0][0]);
	auto ret = Mat<R, C, RetType>::create(R, C);

	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = vec::dot(mat::row(a, r), mat::col(b, c));

	return ret;
}

template<typename T1, typename T2, std::size_t R, std::size_t C>
constexpr auto operator*(const Mat<R, C, T1>& a, const Vec<C, T2>& b)
{
	using RetType = decltype(a[0][0] * b[0] + a[0][0] * b[0]);
	auto ret = Vec<C, RetType> {};

	for(auto r = 0u; r < R; ++r)
		ret[r] = vec::dot(mat::row(a, r), b);

	return ret;
}

template<typename F, typename T, std::size_t R, std::size_t C>
constexpr auto operator*(const F& f, const Mat<R, C, T>& a)
{
	using RetType = decltype(f * a[0][0]);
	auto ret = Mat<R, C, RetType>::create(R, C);

	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = f * a[r][c];

	return ret;
}

template<typename T1, typename T2, std::size_t R, std::size_t C>
constexpr auto operator+(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	using RetType = decltype(a[0][0] + b[0][0]);
	auto ret = Mat<R, C, RetType>::create(R, C);

	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = a[r][c] + b[r][c];

	return ret;
}

template<typename T1, typename T2, std::size_t R, std::size_t C>
constexpr auto operator-(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	using RetType = decltype(a[0][0] + b[0][0]);
	auto ret = Mat<R, C, RetType>::create(R, C);

	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = a[r][c] - b[r][c];

	return ret;
}

template<typename T, std::size_t R, std::size_t C>
constexpr auto operator-(const Mat<R, C, T>& a)
{
	using RetType = decltype(-a[0][0]);
	auto ret = Mat<R, C, RetType>::create(R, C);

	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[r][c] = -a[r][c];

	return ret;
}

template<typename T1, typename T2, std::size_t R, std::size_t C>
constexpr auto operator==(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			if(a[r][c] != b[r][c])
				return false;

	return true;
}

template<typename T1, typename T2, std::size_t R, std::size_t C>
constexpr auto operator!=(const Mat<R, C, T1>& a, const Mat<R, C, T2>& b)
{
	return !(a == b);
}

template<typename T, std::size_t R, std::size_t C>
std::ostream& operator<<(std::ostream& os, const Mat<R, C, T>& a)
{
	return mat::print(os, a);
}

} // namespace nytl

#endif // header guard
