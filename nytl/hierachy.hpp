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

#include <nytl/nonCopyable.hpp>

#include <vector>

namespace nytl
{

template <typename T>
class hierachyNode : public nonMoveable
{
private:
	T* parent_ {nullptr};
	std::vector<T*> children_;

protected:
    virtual void addChild(T& child)
    {
        children_.push_back(&child);
    }

	virtual bool removeChild(T& child) //return if child was found
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

    hierachyNode() = default;
    void create(T& parent)
    {
        parent_ = &parent;
        parent_->addChild((static_cast<T&>(*this)));
    }

    virtual void reparent(T& parent)
    {
        if(parent_) parent_->removeChild(static_cast<T&>(*this));

        parent_ = &parent;
        parent_->addChild((static_cast<T&>(*this)));
    }

public:
    hierachyNode(T& parent){ create(parent); }
	virtual ~hierachyNode(){ destroy(); }

	virtual void destroy()
	{
	    for(auto* c : children_)
        {
            c->parent_ = nullptr; //so they wont call parent_->destroyChild which would invalidate the iterator
            c->destroy();
        }
	    children_.clear();

	    if(parent_) parent_->removeChild(static_cast<T&>(*this));
	    parent_ = nullptr;
    }

	virtual T* getParent() const { return parent_; } //virtual to make it convariant
    virtual bool valid() const { return (parent_ != nullptr) && parent_->valid(); }

	std::vector<T*> getChildren() const { return children_; } //virtual?
	std::size_t getChildrenSize() const { return children_.size(); }
};



}
