#pragma once

#include <memory>

namespace nytl
{

template<typename T>
class potential_ptr
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

protected:
    pointer ptr_ {nullptr};
    bool owned_ {0};

public:
    potential_ptr() = default;
    ~potential_ptr()
    {
        if(owned_ && ptr_)
            delete ptr_;
    }

    //
    potential_ptr(potential_ptr&& other)
        : ptr_(other.ptr_), owned_(other.owned_)
    {
        other.owned_ = 0;
        other.ptr_ = nullptr;
    }

    potential_ptr& operator=(potential_ptr&& other)
    {
        reset(other.ptr_, other.owned_);
        other.owned_ = 0;
        other.ptr_ = nullptr;
        return *this;
    }

    //
    potential_ptr(std::unique_ptr<T>&& other)
        : ptr_(other.release()), owned_(1)
    {
    }

    potential_ptr& operator=(std::unique_ptr<T>&& other)
    {
        reset(other.release(), 1);
        return *this;
    }

    //
    bool owned() const { return owned_; }
    pointer get() const { return ptr_; }

    T& operator*() { return *ptr_; }
    const T& operator*() const { return *ptr_; }

    T* operator->() { return ptr_; }
    const T* operator->() const { return ptr_; }

    //
    pointer release()
    {
        auto ret = ptr_; ptr_ = nullptr; owned_ = 0; return ret;
    }

    //reset
    void reset(pointer p = nullptr, bool owned = 0)
    {
        auto oldp = ptr_;
        auto oldowned = owned_;
        ptr_ = p;
        owned_ = owned;
        if(oldp && oldowned)
            delete oldp;
    }

    void reset(potential_ptr&& other)
    {
        reset(other.ptr_, other.owned_);
        other.owned_ = 0;
        other.ptr_ = nullptr;
    }

    void reset(reference object)
    {
        reset(&object, 0);
    }

    void reset(std::unique_ptr<T>&& other)
    {
        reset(other.release(), 1);
    }

    //swap
    void swap(potential_ptr& other)
    {
        auto myold = ptr_;
        auto myownedold = owned_;

        ptr_ = other.ptr_;
        owned_ = other.owned_;

        other.ptr_ = myold;
        other.owned_ = myownedold;
    }

};

//operators
template<typename T>
potential_ptr<T> copyOrClone(const potential_ptr<T>& ptr)
{
    if(!ptr.owned()) return potential_ptr<T>(ptr.get());
    else return potential_ptr<T>(ptr->clone());
}

}
