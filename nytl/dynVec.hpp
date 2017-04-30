// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Dynamic-sized specialization for nytl::Vec.

#pragma once

#ifndef NYTL_INCLUDE_DYN_VEC
#define NYTL_INCLUDE_DYN_VEC

#include <nytl/fwd/vec.hpp> // nytl::Vec typedefs

#include <cstddef> // std::size_t
#include <iterator> // std::reverse_iterator
#include <vector> // std::vector

namespace nytl {

constexpr auto dynamicSize = 0u;

/// \brief Dynamic-sized nytl::Vec specialization.
template<typename T>
class Vec<dynamicSize, T> : public std::vector<T> {
public:
	using Vector = std::vector<T>;
	using Size = typename Vector::size_type;
	using Value = typename Vector::value_type;
	using Reference = typename Vector::reference;
	using ConstReference = typename Vector::const_reference;

	template<Size OD, typename OT> using Rebind = Vec<OD, OT>;
	static constexpr Vec<dynamicSize, T> create(Size);

	static constexpr auto dim = dynamicSize;

public:
	template<typename OT>
	constexpr explicit operator Vec<dynamicSize, OT>() const;
};

// - implementation -
template<typename T>
constexpr Vec<dynamicSize, T> Vec<dynamicSize, T>::create(Size size)
{
	Vec<dynamicSize,T> ret;
	ret.resize(size);
	return ret;
}

template<typename T>
template<typename OT>
constexpr Vec<dynamicSize, T>::operator Vec<dynamicSize, OT>() const
{
	auto ret = Vec<dynamicSize, OT>::create(this->size());
	for(auto i = 0u; i < std::min(ret.size(), this->size()); ++i)
		ret[i] = (*this)[i];
	for(auto i = std::min(ret.size(), this->size()); i < ret.size(); ++i)
		ret[i] = {};
	return ret;
}

} // namespace nytl

#endif // header guard
