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

template<typename T, typename I = typename T::iterator>
class recursiveIterator : public std::iterator<std::bidirectional_iterator_tag, T, std::size_t>
{
public:
	I it_;
	recursiveIterator<T, I> child_;
	bool onParent_ {1};

public:
	recursiveIterator(const I& it) : it_(it) {}

	auto operator->() -> decltype(&(*it_)) { return onParent_ ? &(*it_) : &(*child_); }
	auto operator*() -> decltype(*it_) { return onParent_ ? *it_ : *child_; }

	recursiveIterator& operator++()
	{ 
		if(onParent_)
		{
			child_ = it_->recursive_begin();
			if(child_ == it_->recursive_end())
			{
				++it_;
			}
			else
			{
				onParent_ = 0;
			}
		}
		else if(child_++ == it_->recursive_end())
		{
			++it_;
			onParent_ = 1;
		}

		return *this; 
	}

	recursiveIterator operator++(int)
	{
		auto cop = *this;
		++(*this);
		return cop;
	}

	recursiveIterator& operator--()
	{
		if(onParent_)
		{
			--it_;

			child_ = it_->recusive_end();
			if(child_ == it_->recursive_begin()) //empty
			{
				--it_;
			}
			else
			{
				--child_;
				onParent_ = 0;
			}
		}
		else if(child_ != it_->recursive_begin())
		{
			--child_;
		}
		else
		{
			--it_;
			onParent_ = 1;
		}

		return *this;
	}
	
	recursiveIterator operator--(int)
	{
		auto cop = *this;
		--(*this);
		return cop;
	}
};

//const
template<typename T> 
using constRecursiveIterator = recursiveIterator<T, typename T::const_iterator>;


//iteration
template<typename T>
class recursiveIteration
{
public:
	using iterator = recursiveIterator<T>;
	using const_iterator = constRecursiveIterator<T>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	T* object_;

public:
	recursiveIteration(T& object) : object_(&object) {}

	iterator begin(){ object_->recursive_begin(); }	
	const_iterator begin() const { return object_->recursive_begin(); }
	const_iterator cbegin() const { return object_->recursive_cbegin(); }
	reverse_iterator rbegin() { return object_->recursive_rbegin(); }
	const_reverse_iterator rbegin() const { return object_->recursive_rbegin(); }
	const_reverse_iterator crbegin() const { return object_->recursive_crbegin(); }

	iterator end(){ object_->recursive_end(); }	
	const_iterator end() const { return object_->recursive_end(); }
	const_iterator cend() const { return object_->recursive_cend(); }
	reverse_iterator rend(){ return object_->recursive_rend(); }
	const_reverse_iterator rend() const { return object_->recursive_rend(); }
	const_reverse_iterator crend() const { return object_->recursive_crend(); }
};

}
