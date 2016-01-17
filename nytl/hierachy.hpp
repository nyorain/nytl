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

///\file
///\brief Contains a template class for creating hierachy-structered clas objects.

#pragma once

#include <nytl/nonCopyable.hpp>
#include <nytl/referenceIteration.hpp>
#include <nytl/bits/recursiveIteration.inl>
#include <vector>

namespace nytl
{

///\brief Virtual utility base template class for objects that are part of a hierachy.
///\ingroup utility
template <typename T>
class hierachyNode
{
public:
	using node_type = hierachyNode<T>;
	using vector_type = std::vector<T*>;

	using iterator = referenceIterator<typename vector_type::iterator>;
	using const_iterator = referenceIterator<typename vector_type::const_iterator>;
	using reverse_iterator = referenceIterator<typename vector_type::reverse_iterator>;
	using const_reverse_iterator = referenceIterator<typename vector_type::const_reverse_iterator>;

	using rec_iterator = recursiveIterator<node_type>;
	using const_rec_iterator = constRecursiveIterator<node_type>;
	using reverse_rec_iterator = std::reverse_iterator<rec_iterator>;
	using const_reverse_rec_iterator = std::reverse_iterator<const_rec_iterator>;

	using recursive_iteration = recursiveIteration<node_type>;
		
private:
	vector_type children_;
	T* parent_ {nullptr};

protected:
    hierachyNode() = default;
    hierachyNode(T& parent){ create(parent); }
	virtual ~hierachyNode(){ destroy(); }

    virtual void addChild(T& child) { children_.push_back(&child); }
	virtual bool removeChild(T& child)	
	{
	    for(auto it = children_.cbegin(); it != children_.cend(); ++it)
	    {
            if(*it == &child)
            {
                children_.erase(it);
                return 1;
            }
        }
	    return 0;
	}

	virtual void create(T& parent)
	{
		parent_ = &parent;
		parent_->addChild(static_cast<T&>(*this));
	}
    virtual void reparent(T& parent)
	{  
        if(parent_) parent_->removeChild(static_cast<T&>(*this));

        parent_ = &parent;
        parent_->addChild((static_cast<T&>(*this)));
	}
	virtual void destroy()
	{
	    for(auto* c : children_)
        {
            c->parent_ = nullptr; 
            c->destroy();
        }

	    children_.clear();

	    if(parent_) 
		{
			parent_->removeChild(static_cast<T&>(*this));
			parent_ = nullptr;
		}
    };

public:
	//iterator
	iterator begin(){ return children_.begin(); }
	const_iterator begin() const { return children_.cbegin(); }
	const_iterator cbegin() const { return children_.cbegin(); }
	reverse_iterator rbegin(){ return children_.rbegin(); }
	const_reverse_iterator rbegin() const { return children_.crbegin(); }
	const_reverse_iterator crbegin() const { return children_.crbegin(); }

	iterator end(){ return children_.end(); }
	const_iterator end() const { return children_.cend(); }
	const_iterator cend() const { return children_.cend(); }
	reverse_iterator rend(){ return children_.rend(); }
	const_reverse_iterator rend() const { return children_.crend(); }
	const_reverse_iterator crend() const { return children_.crend(); }

	//recursive_iterator
	rec_iterator recursive_begin()
		{ return rec_iterator(children_.begin()); }
	const_rec_iterator recursive_begin() const 
		{ return const_rec_iterator(children_.cbegin()); }
	const_rec_iterator recursive_cbegin() const 
		{ return const_rec_iterator(children_.cbegin()); }
	reverse_rec_iterator recursive_rbegin()
		{ return reverse_rec_iterator(children_.rbegin()); }
	const_reverse_rec_iterator recursive_rbegin() const
		{ return const_reverse_rec_iterator(children_.crbegin()); }
	const_reverse_rec_iterator recursive_crbegin() const
		{ return const_reverse_rec_iterator(children_.crbegin()); }

	rec_iterator recursive_end() { return children_.empty() ? 
		rec_iterator(children_.end()) : children_[0]->recursive_end(); }
	const_rec_iterator recursive_end() const { return children_.empty() ?
		const_rec_iterator(children_.cend()) : children_[0]->recursive_cend(); }
	const_rec_iterator recursive_cend() const { return children_.empty() ?
		const_rec_iterator(children_.cend()) : children_[0]->recursive_cend(); }
	reverse_rec_iterator recursive_rend() { return children_.empty() ?
		reverse_rec_iterator(children_.rend()) : children_[0]->recursive_rend(); }
	const_reverse_rec_iterator recursive_rend() const { return children_.empty() ?
		const_reverse_rec_iterator(children_.crend()) : children_[0]->recursive_crend(); }
	const_reverse_rec_iterator recursive_crend() const { return children_.empty() ?
		const_reverse_rec_iterator(children_.crend()) : children_[0]->recursive_crend(); }

	//
	recursive_iteration recursive() { return recursive_iteration(*this); }
	const recursive_iteration recursive() const { return recursive_iteration(*this); }

	//
	virtual T* parent() const { return parent_; } //virtual to make it convariant
    virtual bool valid() const { return (parent_ != nullptr) && parent_->valid(); }

	const vector_type& children() const { return children_; } 
	std::size_t childrenCount() const { return children_.size(); }
};
///\example xml.cpp A simple example on how to use the nytl::hierachy templates.

//todo, correct design?
template<typename T>
class hierachyRoot : public T
{
private:
	virtual void create(T&) override {}
	virtual void reparent(T&) override {}
	virtual T* parent() const override { return nullptr; }

public:
	hierachyRoot() = default;

	virtual bool valid() const override { return 1; }
};

}
