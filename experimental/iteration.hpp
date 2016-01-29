template<typename T>
class recursiveIterator
{
public:
	typename T::iterator it_;
	recursiveIterator<T> child_;
	bool onParent_ {1};

public:
	recursiveIterator(typename T::iterator it) : it_(it) {}

	T* operator->(){ return onParent_ ? &(*it_) : &(*child_); }
	T& operator*(){ return onParent_ ? *it_ : *child_; }

	recursiveIterator& operator++()
	{ 
		if(onParent_)
		{
			child_ = it_.recursive_begin();
			if(child_ == it_.recursive_end())
			{
				it_++;
				onParent_ = 1;
			}
			else
			{
				onParent_ = 0;
			}
		}
		else if(child_ != it_.recursive_end())
		{
			child_++;
		}
		else
		{
			it_++;
			onParent_ = 1;
		}

		return *this; 
	}
};
