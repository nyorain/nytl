#pragma once

#include <nyutil/nonCopyable.hpp>

#include <vector>
#include <stdexcept>

namespace nyutil
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
        if(!child.valid() || child.getParent() != this)
            throw std::logic_error("hierachyNode::addChild: invalid child");

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
    void create(T& parent) //can be used to reparent the object
    {
        if(!parent.valid())
            throw std::logic_error("hierachyNode::create: invalid parent");

        if(parent_)
        {
            parent_->removeChild(static_cast<T&>(*this));
            parent_ = nullptr;
        }

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

	virtual T* getParent() const { return parent_; }
    virtual bool valid() const { return parent_ != nullptr; }

	std::vector<T*> getChildren() const { return children_; } //virtual?
	size_t getChildrenSize() const { return children_.size(); }
};



}
