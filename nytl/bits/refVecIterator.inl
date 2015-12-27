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

namespace nytl
{

template<size_t dim, typename T>
class const_refvec_iterator : public std::iterator<std::random_access_iterator_tag, T, size_t>
{
protected:
    const vec<dim, T&>& ref_;
    int64_t idx_;

public:
    const_refvec_iterator(const vec<dim, T&>& c, int64_t idx = 0) : ref_(c), idx_(idx) {}

    const T* operator->(){ return &ref_[idx_]; }
    const T& operator*(){ return ref_[idx_]; }

    bool operator==(const const_refvec_iterator& other) const 
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const const_refvec_iterator& other) const 
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    const_refvec_iterator& operator++(){ idx_++; return *this; }
    const_refvec_iterator& operator++(int){ auto cop = *this; idx_++; return cop; }

    const_refvec_iterator& operator--(){ idx_--; return *this; }
    const_refvec_iterator& operator--(int){ auto cop = *this; idx_--; return cop; }

    const_refvec_iterator& operator+=(size_t i){ idx_ += i; return *this; }
    const_refvec_iterator& operator-=(size_t i){ idx_ -= i; return *this; }

    //custom
    int64_t getIndex() const { return idx_; }
    const vec<dim, T&>& getRef() const {return ref_; }
};

//non-const
template<size_t dim, typename T>
class refvec_iterator : public std::iterator<std::random_access_iterator_tag, T, size_t>
{
protected:
    vec<dim, T&>& ref_;
    int64_t idx_; //can be -1, if it points to the pre-first element

public:
    refvec_iterator(vec<dim, T&>& c, int64_t idx = 0) : ref_(c), idx_(idx) {}

    T* operator->(){ return &ref_[idx_]; }
    T& operator*(){ return ref_[idx_]; }

    bool operator==(const refvec_iterator& other) const 
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const refvec_iterator& other) const 
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    refvec_iterator& operator++(){ idx_++; return *this; }
    refvec_iterator& operator++(int){ auto cop = *this; idx_++; return cop; }

    refvec_iterator& operator--(){ idx_--; return *this; }
    refvec_iterator& operator--(int){ auto cop = *this; idx_--; return cop; }

    refvec_iterator& operator+=(size_t i){ idx_ += i; return *this; }
    refvec_iterator& operator-=(size_t i){ idx_ -= i; return *this; }

    operator const_refvec_iterator<dim, T>(){ return const_refvec_iterator<dim, T>(ref_, idx_); }

    //custom
    int64_t getIndex() const { return idx_; }
    vec<dim, T&>& getRef() const {return ref_; }
};

}
