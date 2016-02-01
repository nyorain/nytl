#include <vector>

template<typename Root, typename Child>
class HierachyBase
{
public:
	using root_type = Root;
	using child_type = Child;

protected:
	std::vector<Child*> children_;

protected:
	virtual void destroy()
	{
	    for(auto* c : children_)
        {
            c->parent_ = nullptr; 
            c->destroy();
        }

	    children_.clear();

    };

	void addChild(Child& child){ children_.push_back(&child); }
	bool removeChild(Child& child)	
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

public:
	virtual ~HierachyBase() { destroy(); };
	virtual Root& root() const = 0;
};

template<typename T>
class HierachyNode : public T
{
public:
	using typename T::root_type;

protected:
	T* parent_;

	virtual void destroy() override
	{
		T::destroy();
	    if(parent_) 
		{
			parent_->removeChild(static_cast<T&>(*this));
			parent_ = nullptr;
		}
	}

public:
	HierachyNode(T& parent) : parent_(*parent) {}

	virtual root_type& root() const override { return parent_->root(); }
	T& parent() const { return *parent_; }
};

template<typename T>
class HierachyRoot : public T
{
public:
	using typename T::root_type;

public:
	virtual root_type& root() const override { return static_cast<root_type&>(*this); }
};


//
class xmlNode;
class xmlRoot;

class xmlNodeBase : public HierachyBase<xmlRoot, xmlNode>
{
};

class xmlRoot : public HierachyRoot<xmlNodeBase>
{
};

class xmlNode : public HierachyNode<xmlNodeBase>
{
};

class generalXmlNode : public HierachyNode<HierachyBase<generalXmlNode, generalXmlNode>>
{
};

