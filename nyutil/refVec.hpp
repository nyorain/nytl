#pragma once

#include <nyutil/vec.hpp>

namespace nyutil
{

//todo
//specializations
//operators (like vec)

//is p/typedefs
template<size_t dim, typename T> using refVec = vec<dim, T&>;

template<typename T> using refVec2 = vec<2, T&>;
template<typename T> using refVec3 = vec<3, T&>;
template<typename T> using refVec4 = vec<4, T&>;

typedef refVec2<float> refVec2f;
typedef refVec2<int> refVec2i;
typedef refVec2<unsigned int> refVec2ui;
typedef refVec2<double> refVec2d;
typedef refVec2<char> refVec2c;
typedef refVec2<unsigned char> refVec2uc;
typedef refVec2<long> refVec2l;
typedef refVec2<unsigned long> refVec2ul;

typedef refVec3<float> refVec3f;
typedef refVec3<int> refVec3i;
typedef refVec3<unsigned int> refVec3ui;
typedef refVec3<double> refVec3d;
typedef refVec3<char> refVec3c;
typedef refVec3<unsigned char> refVec3uc;
typedef refVec3<long> refVec3l;
typedef refVec3<unsigned long> refVec3ul;

typedef refVec4<float> refVec4f;
typedef refVec4<int> refVec4i;
typedef refVec4<unsigned int> refVec4ui;
typedef refVec4<double> refVec4d;
typedef refVec4<char> refVec4c;
typedef refVec4<unsigned char> refVec4uc;
typedef refVec4<long> refVec4l;
typedef refVec4<unsigned long> refVec4ul;

//specialization
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

    bool operator==(const const_refvec_iterator& other) const { return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const const_refvec_iterator& other) const { return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

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

    bool operator==(const refvec_iterator& other) const { return (&ref_ == &other.ref_) && (idx_ == other.idx_); }
    bool operator!=(const refvec_iterator& other) const { return (&ref_ != &other.ref_) || (idx_ != other.idx_); }

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


//vec
template<size_t dimension, typename T> class vec<dimension, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = dimension;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = refvec_iterator<dim, value_type>;
    using const_iterator = const_refvec_iterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using vec_type = vec<dimension, reference>;
    using normal_vec_type = vec<dimension, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    pointer data_[dim];

public:
    template<typename... Args, typename = typename std::enable_if<std::is_convertible<std::tuple<Args...>, typename typeTuple<value_type, dim>::type>::value>::type>
    vec(Args&&... args) : data_{std::forward<Args>(&args)...} {}
    ~vec() noexcept = default;

    vec(const vec_type& other) noexcept = default;
    vec(vec_type&& other) noexcept = default;

    vec(const normal_vec_type& other) noexcept { for(size_t i(0); i < dim; i++) data_[i] = &other[i]; } //copies pointer
    vec_type& operator=(const normal_vec_type& other) noexcept { for(size_t i(0); i < dim; i++) *data_[i] = other[i]; return *this; } //copies value
    vec_type& operator=(const vec_type& other) noexcept { for(size_t i(0); i < dim; i++) *data_[i] = other[i]; return *this; } //copies value

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator >>=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator <<=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other; return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> vec_type& operator >>=(const ot& other){ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> vec_type& operator <<=(const ot& other){ for(auto& val : *this) val <<= other; return *this; }

    vec_type operator-() const { vec_type ret(*this); for(size_t i(0); i < dim; i++) ret[i] -= (*this)[i]; return ret; }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return data_; }
    pointer data() noexcept { return data_; }

    void fill(const value_type& val) { for(auto& v : data_) v = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ return *data_[i]; }
    const_reference operator[](size_type i) const { return *data_[i]; }

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return *data_[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return *data_[i]; }

    reference front() noexcept { return *data_[0]; }
    const_reference front() const noexcept { return *data_[0]; }

    reference back() noexcept { return *data_[dim - 1]; }
    const_reference back() const noexcept { return *data_[dim - 1]; }
};


//2
//vec
template<typename T> class vec<2, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 2;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = refvec_iterator<dim, value_type>;
    using const_iterator = const_refvec_iterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using vec_type = vec<dim, reference>;
    using normal_vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;

public:
    vec(reference a, reference b) : x(a), y(b) {}
    ~vec() noexcept = default;

    vec(const vec_type& other) noexcept = default;
    vec(vec_type&& other) noexcept = default;

    vec(const normal_vec_type& other) noexcept : x(other.x), y(other.y) {} //copies pointer
    vec_type& operator=(const normal_vec_type& other) noexcept { x = other.x; y = other.y; return *this; return *this; } //copies value
    vec_type& operator=(const vec_type& other) noexcept { x = other.x; y = other.y; return *this; return *this; } //copies value

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator >>=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator <<=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other; return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> vec_type& operator >>=(const ot& other){ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> vec_type& operator <<=(const ot& other){ for(auto& val : *this) val <<= other; return *this; }

    normal_vec_type operator-() const { return normal_vec_type(x, y); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ if(i == 0) return x; return y; }
    const_reference operator[](size_type i) const { if(i == 0) return x; return y; }

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return y; }
    const_reference back() const noexcept { return y; }
};

//3
//vec
template<typename T> class vec<3, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 3;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = refvec_iterator<dim, value_type>;
    using const_iterator = const_refvec_iterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using vec_type = vec<dim, reference>;
    using normal_vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;
    reference z;

public:
    vec(reference a, reference b, reference c) noexcept : x(a), y(b), z(c) {}
    ~vec() noexcept = default;

    vec(const vec_type& other) noexcept = default;
    vec(vec_type&& other) noexcept = default;

    vec(const normal_vec_type& other) noexcept : x(other.x), y(other.y), z(other.z) { } //copies pointer
    vec_type& operator=(const normal_vec_type& other) noexcept { x = other.x; y = other.y; z = other.z; return *this; } //copies value
    vec_type& operator=(const vec_type& other) noexcept { x = other.x; y = other.y; z = other.z; return *this; } //copies value

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator >>=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator <<=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other; return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> vec_type& operator >>=(const ot& other){ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> vec_type& operator <<=(const ot& other){ for(auto& val : *this) val <<= other; return *this; }

    normal_vec_type operator-() const { return normal_vec_type(x, y, z); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ if(i == 0) return x; if(i == 1) return y; return z; }
    const_reference operator[](size_type i) const { if(i == 0) return x; if(i == 1) return y; return z; }

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return z; }
    const_reference back() const noexcept { return z; }

    //custom
    vec2<reference> xy() const noexcept { return vec2<reference>(x,y); }
    vec2<reference> yz() const noexcept { return vec2<reference>(y,z); }
    vec2<reference> xz() const noexcept { return vec2<reference>(x,z); }
};


//4
//vec
template<typename T> class vec<4, T&>
{
public:
    using value_type = T;
    constexpr static size_t dim = 4;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = refvec_iterator<dim, value_type>;
    using const_iterator = const_refvec_iterator<dim, value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using vec_type = vec<dim, reference>;
    using normal_vec_type = vec<dim, value_type>;

public:
    constexpr size_t size() const noexcept { return dim; }
    constexpr size_t length() const noexcept { return dim; }
    constexpr size_t max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    reference x;
    reference y;
    reference z;
    reference w;

public:
    vec(reference a, reference b, reference c, reference d) noexcept : x(a), y(b), z(c), w(d) {}
    ~vec() noexcept = default;

    vec(const vec_type& other) noexcept = default;
    vec(vec_type&& other) noexcept = default;

    vec(const normal_vec_type& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {} //copies pointer
    vec_type& operator=(const normal_vec_type& other) noexcept { x = other.x; y = other.y; z = other.z; w = other.w; return *this; } //copies value
    vec_type& operator=(const vec_type& other) noexcept { x = other.x; y = other.y; z = other.z; w = other.w; return *this; } //copies value

    //operator
    template <size_t odim, typename ot> vec_type& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] %= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator |=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] |= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator ^=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] ^= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator &=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] &= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator >>=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] >>= other[i]; return *this; }
    template <size_t odim, typename ot> vec_type& operator <<=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) (*this)[i] <<= other[i]; return *this; }

    template<typename ot> vec_type& operator +=(const ot& other){ for(auto& val : *this) val += other; return *this; }
    template<typename ot> vec_type& operator -=(const ot& other){ for(auto& val : *this) val -= other; return *this; }
    template<typename ot> vec_type& operator *=(const ot& other){ for(auto& val : *this) val *= other; return *this; }
    template<typename ot> vec_type& operator /=(const ot& other){ for(auto& val : *this) val /= other; return *this; }
    template<typename ot> vec_type& operator %=(const ot& other){ for(auto& val : *this) val %= other; return *this; }
    template<typename ot> vec_type& operator |=(const ot& other){ for(auto& val : *this) val |= other; return *this; }
    template<typename ot> vec_type& operator ^=(const ot& other){ for(auto& val : *this) val ^= other; return *this; }
    template<typename ot> vec_type& operator &=(const ot& other){ for(auto& val : *this) val &= other; return *this; }
    template<typename ot> vec_type& operator >>=(const ot& other){ for(auto& val : *this) val >>= other; return *this; }
    template<typename ot> vec_type& operator <<=(const ot& other){ for(auto& val : *this) val <<= other; return *this; }

    normal_vec_type operator-() const { return normal_vec_type(x, y, z); }

    template <size_t odim, typename ot, typename = typename std::enable_if<!std::is_reference<ot>::value>::type>
    operator vec<odim, ot>() const { vec<odim, ot> ret; ret.fill(ot()); for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = (*this)[i]; return ret; }

    //stl norm stuff, std::array
    const_pointer data() const noexcept { return &x; }
    pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; w = val; }

    iterator begin() noexcept { return iterator(*this); }
    const_iterator begin() const noexcept { return const_iterator(*this); }
    const_iterator cbegin() const noexcept { return const_iterator(*this); }

    iterator end() noexcept { return iterator(*this, dim); }
    const_iterator end() const noexcept { return const_iterator(*this, dim); }
    const_iterator cend() const noexcept { return const_iterator(*this, dim); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    reference operator[](size_type i){ if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }
    const_reference operator[](size_type i) const { if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }

    reference at(size_type i){ if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }
    const_reference at(size_type i) const { if(i >= dim || i < 0) throw std::out_of_range("nyutil::vec::at: out of range"); return (*this)[i]; }

    reference front() noexcept { return x; }
    const_reference front() const noexcept { return x; }

    reference back() noexcept { return w; }
    const_reference back() const noexcept { return w; }

    //custom
    vec2<reference> xy() const noexcept { return vec2<reference>(x,y); }
    vec2<reference> xz() const noexcept { return vec2<reference>(x,z); }
    vec2<reference> xw() const noexcept { return vec2<reference>(x,w); }
    vec2<reference> yz() const noexcept { return vec2<reference>(y,z); }
    vec2<reference> yw() const noexcept { return vec2<reference>(y,w); }
    vec2<reference> zw() const noexcept { return vec2<reference>(z,w); }

    vec3<reference> xyz() const noexcept { return vec3<reference>(x,y,z); }
    vec3<reference> xyw() const noexcept { return vec3<reference>(x,y,w); }
    vec3<reference> xzw() const noexcept { return vec3<reference>(x,z,w); }
    vec3<reference> yzw() const noexcept { return vec3<reference>(y,z,w); }
};

//
template<size_t dim, typename T> constexpr size_t vec<dim, T&>::dim;
template<typename T> constexpr size_t vec<2, T&>::dim;
template<typename T> constexpr size_t vec<3, T&>::dim;
template<typename T> constexpr size_t vec<4, T&>::dim;

//invalid
template<typename T> class vec<0, T>;

}

