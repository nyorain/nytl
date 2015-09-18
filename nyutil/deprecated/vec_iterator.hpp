#pragma once

namespace nyutil
{

//vec iterator
//define them outside of the vec classes because they require to specialization for vec2/3/4 -> no code duplication
//const iterator
template<size_t dim, typename T>
class const_vec_iterator : public std::iterator<std::random_access_iterator_tag, T, size_t>
{
protected:
    const vec<dim, T>& ref_;
    int64_t idx_;

public:
    vecIterator(const vec<dim, T>& c, int64_t idx = 0) : curr_(c), idx(i) {}

    const T* operator->(){ return &ref_[idx_]; }
    const T& operator*(){ return ref_[idx_]; }

    bool operator==(const const_vec_iterator& other) const { return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const const_vec_iterator& other) const { return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    const_vec_iterator& operator++(){ idx_++; return *this; }
    const_vec_iterator& operator++(int){ auto cop = *this; idx_++; return cop; }

    const_vec_iterator& operator--(){ idx_--; return *this; }
    const_vec_iterator& operator--(int){ auto cop = *this; idx_--; return cop; }

    const_vec_iterator& operator+=(size_t i){ idx_ += i; return *this; }
    const_vec_iterator& operator-=(size_t i){ idx_ -= i; return *this; }

    //custom
    int64_t getIndex() const { return idx_; }
    const vec<dim, T>& getRef() const {return ref_; }
};

//non-const
template<size_t dim, typename T>
class vec_iterator : public std::iterator<std::random_access_iterator_tag, T, size_t>
{
protected:
    vec<dim, T>& ref_;
    int64_t idx_; //can be -1, if it points to the pre-first element

public:
    vecIterator(vec<dim, T>& c, int64_t idx = 0) : curr_(c), idx(i) {}

    T* operator->(){ return &ref_[idx_]; }
    T& operator*(){ return ref_[idx_]; }

    bool operator==(const vec_iterator& other) const { return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const vec_iterator& other) const { return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

    vec_iterator& operator++(){ idx_++; return *this; }
    vec_iterator& operator++(int){ auto cop = *this; idx_++; return cop; }

    vec_iterator& operator--(){ idx_--; return *this; }
    vec_iterator& operator--(int){ auto cop = *this; idx_--; return cop; }

    vec_iterator& operator+=(size_t i){ idx_ += i; return *this; }
    vec_iterator& operator-=(size_t i){ idx_ -= i; return *this; }

    operator const_vec_iterator<dim, T>(){ return const_vec_iterator(ref_, idx_); }

    //custom
    int64_t getIndex() const { return idx_; }
    vec<dim, T>& getRef() const {return ref_; }
};

}
