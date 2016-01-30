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

#include <iterator>

namespace nytl
{

template<size_t dim, typename T>
class constRefVecIterator : public std::iterator<std::random_access_iterator_tag, T, std::size_t>
{
protected:
    const Vec<dim, T&>& ref_;
    int64_t idx_;

public:
    constRefVecIterator(const Vec<dim, T&>& c, int64_t idx = 0) : ref_(c), idx_(idx) {}

    const T* operator->(){ return &ref_[idx_]; }
    const T& operator*(){ return ref_[idx_]; }

    bool operator==(const constRefVecIterator& other) const 
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const constRefVecIterator& other) const 
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    constRefVecIterator& operator++(){ idx_++; return *this; }
    constRefVecIterator operator++(int){ auto cop = *this; idx_++; return cop; }

    constRefVecIterator& operator--(){ idx_--; return *this; }
    constRefVecIterator operator--(int){ auto cop = *this; idx_--; return cop; }

    constRefVecIterator& operator+=(size_t i){ idx_ += i; return *this; }
    constRefVecIterator operator-=(size_t i){ idx_ -= i; return *this; }

    //custom
    int64_t getIndex() const { return idx_; }
    const Vec<dim, T&>& getRef() const {return ref_; }
};

//non-const
template<size_t dim, typename T>
class refVecIterator : public std::iterator<std::random_access_iterator_tag, T, size_t>
{
protected:
    Vec<dim, T&>& ref_;
    int64_t idx_; //can be -1, if it points to the pre-first element

public:
    refVecIterator(Vec<dim, T&>& c, int64_t idx = 0) : ref_(c), idx_(idx) {}

    T* operator->(){ return &ref_[idx_]; }
    T& operator*(){ return ref_[idx_]; }

    bool operator==(const refVecIterator& other) const 
		{ return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const refVecIterator& other) const 
		{ return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    refVecIterator& operator++(){ idx_++; return *this; }
    refVecIterator& operator++(int){ auto cop = *this; idx_++; return cop; }

    refVecIterator& operator--(){ idx_--; return *this; }
    refVecIterator& operator--(int){ auto cop = *this; idx_--; return cop; }

    refVecIterator& operator+=(size_t i){ idx_ += i; return *this; }
    refVecIterator& operator-=(size_t i){ idx_ -= i; return *this; }

    operator constRefVecIterator<dim, T>(){ return constRefVecIterator<dim, T>(ref_, idx_); }

    //custom
    int64_t getIndex() const { return idx_; }
    Vec<dim, T&>& getRef() const {return ref_; }
};

}
