template<typename T>
class RecursiveIterator
{
public:
	typename T::iterator it_;
	RecursiveIterator<T> child_;
	bool onParent_ {1};

public:
	RecursiveIterator(typename T::iterator it) : it_(it) {}

	T* operator->(){ return onParent_ ? &(*it_) : &(*child_); }
	T& operator*(){ return onParent_ ? *it_ : *child_; }

	RecursiveIterator& operator++()
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
