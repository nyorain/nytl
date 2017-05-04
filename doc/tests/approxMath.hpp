// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// Approx specialization for nytl math types like nytl::Vec or nytl::Mat.

#pragma once

#include "approx.hpp" // nytl::Approx
#include <nytl/vec.hpp> // nytl::Vec
#include <nytl/mat.hpp> // nytl::Mat


namespace nytl {

/// Approx specialization for nytl::Vec
template<size_t D, typename T>
class Approx<nytl::Vec<D, T>> {
public:
	template<size_t D2, typename T2>
	friend bool operator==(const nytl::Vec<D2, T2>& lhs, const Approx& rhs)
	{
		if(lhs.size() != rhs.value.size())
			return false;

		for(auto i = 0u; i < lhs.size(); ++i)
			if(lhs[i] != approx(rhs.value[i], rhs.epsilon))
				return false;

		return true;
	}

	template<size_t D2, typename T2>
	friend bool operator==(const Approx& lhs, const nytl::Vec<D2, T2>& rhs)
		{ return operator==(rhs, lhs); }

	template<size_t D2, typename T2>
	friend bool operator!=(const nytl::Vec<D2, T2>& lhs, const Approx& rhs)
		{ return !operator==(lhs, rhs); }

	template<size_t D2, typename T2>
	friend bool operator!=(const Approx& lhs, const nytl::Vec<D2, T2>& rhs)
		{ return !operator==(lhs, rhs); }

public:
	nytl::Vec<D, T> value {};
	double epsilon {defaultApproxEpsilon};
};

/// Approx specialization for nytl::Mat
template<size_t R, size_t C, typename T>
class Approx<nytl::Mat<R, C, T>> {
public:
	template<typename T2>
	friend bool operator==(const nytl::Mat<R, C, T2>& lhs, const Approx& rhs)
	{
		if(lhs.rows() != rhs.value.rows())
			return false;

		for(auto i = 0u; i < lhs.rows(); ++i)
			if(lhs[i] != approx(rhs.value[i], rhs.epsilon))
				return false;

		return true;
	}

	template<typename T2>
	friend bool operator==(const Approx& lhs, const nytl::Mat<R, C, T2>& rhs)
		{ return operator==(rhs, lhs); }

	template<typename T2>
	friend bool operator!=(const nytl::Mat<R, C, T2>& lhs, const Approx& rhs)
		{ return !operator==(lhs, rhs); }

	template<typename T2>
	friend bool operator!=(const Approx& lhs, const nytl::Mat<R, C, T2>& rhs)
		{ return !operator==(lhs, rhs); }

public:
	nytl::Mat<R, C, T> value {};
	double epsilon {defaultApproxEpsilon};
};

template<typename T>
Approx<T> approx(const T& value, double epsilon)
{
	return {value, epsilon};
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
	os << "Approx(" << approx.value << ")";
	return os;
}

} // namesapce nytl
