// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_REFVECITERATOR_INL
#define NYTL_INCLUDE_REFVECITERATOR_INL

#include <iterator>

namespace nytl
{

template<std::size_t dim, typename T>
class ConstRefVecIterator
	: public std::iterator<std::random_access_iterator_tag, T, std::size_t>
{
protected:
	const Vec<dim, T&>& ref_;
	std::int64_t idx_;

public:
	ConstRefVecIterator(const Vec<dim, T&>& c, std::int64_t idx = 0) : ref_(c), idx_(idx) {}

	const T* operator->(){ return &ref_[idx_]; }
	const T& operator*(){ return ref_[idx_]; }

	bool operator==(const ConstRefVecIterator& other) const
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
	bool operator!=(const ConstRefVecIterator& other) const
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

	ConstRefVecIterator& operator++(){ idx_++; return *this; }
	ConstRefVecIterator operator++(int){ auto cop = *this; idx_++; return cop; }

	ConstRefVecIterator& operator--(){ idx_--; return *this; }
	ConstRefVecIterator operator--(int){ auto cop = *this; idx_--; return cop; }

	ConstRefVecIterator& operator+=(std::size_t i){ idx_ += i; return *this; }
	ConstRefVecIterator operator-=(std::size_t i){ idx_ -= i; return *this; }

	//custom
	std::int64_t index() const { return idx_; }
	const Vec<dim, T&>& ref() const {return ref_; }
};

//non-const
template<std::size_t dim, typename T>
class RefVecIterator
	: public std::iterator<std::random_access_iterator_tag, T, std::size_t>
{
protected:
	Vec<dim, T&>& ref_;
	std::int64_t idx_; //can be -1, if it points to the pre-first element

public:
	RefVecIterator(Vec<dim, T&>& c, std::int64_t idx = 0) : ref_(c), idx_(idx) {}

	T* operator->(){ return &ref_[idx_]; }
	T& operator*(){ return ref_[idx_]; }

	bool operator==(const RefVecIterator& other) const
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
	bool operator!=(const RefVecIterator& other) const
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

	RefVecIterator& operator++(){ idx_++; return *this; }
	RefVecIterator& operator++(int){ auto cop = *this; idx_++; return cop; }

	RefVecIterator& operator--(){ idx_--; return *this; }
	RefVecIterator& operator--(int){ auto cop = *this; idx_--; return cop; }

	RefVecIterator& operator+=(std::size_t i){ idx_ += i; return *this; }
	RefVecIterator& operator-=(std::size_t i){ idx_ -= i; return *this; }

	operator ConstRefVecIterator<dim, T>(){ return ConstRefVecIterator<dim, T>(ref_, idx_); }

	//custom
	std::int64_t index() const { return idx_; }
	Vec<dim, T&>& ref() const {return ref_; }
};

}

#endif //header guard
