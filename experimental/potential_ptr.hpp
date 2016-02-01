/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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
///\brief Smart pointer class potential_ptr which can handle owned as well as non-owned objects.

//TODO: not really needed? not the sence of a smart pointer ~

#pragma once

#include <memory>

namespace nytl
{

///\brief Can handle an owned or with an non-owned object.
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
    potential_ptr(std::unique_ptr<T>&& value)
        : ptr_(value.release()), owned_(1)
    {
    }

    potential_ptr(T& value) noexcept
        : ptr_(&value), owned_(0)
    {
    }

    potential_ptr& operator=(std::unique_ptr<T>&& value)
    {
        reset(value.release(), 1);
        return *this;
    }

    potential_ptr& operator=(T& value)
    {
        reset(&value, 0);
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
