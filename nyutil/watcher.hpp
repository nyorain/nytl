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

#include <nyutil/callback.hpp>

namespace nyutil
{

class watchable
{
protected:
    callback<void()> destructionCallback_;

public:
    ~watchable(){ destructionCallback_(); } //virtual?
    connection onDestruction(std::function<void()> func){ return destructionCallback_.add(func); }
};

//ref//////////////////////////////////////////////
//move, copy semantics
template <typename T = watchable>
class watcherRef
{
protected:
    T* ref_;
    connection conn_;

public:
    watcherRef() = default;
    watcherRef(T& nref) { set(nref); }
    ~watcherRef() { if(ref_) conn_.destroy(); }

    watcherRef(const watcherRef<T>& other) : ref_(other.ref_) { if(ref_) set(*ref_); }
    watcherRef& operator=(const watcherRef<T>& other) { reset(); if(other.ref_) set(*other.ref_); return *this; }

    watcherRef(watcherRef<T>&& other) : ref_(other.ref_), conn_(std::move(other.conn_)) { other.ref_ = nullptr; }
    watcherRef& operator=(watcherRef<T>&& other) { reset(); ref_ = other.ref_; conn_ = std::move(other.conn_); other.ref_ = nullptr; }

    T* get() const { return ref_; }
    void set(T& nref)
    {
        if(&nref == ref_) return;
        reset();

        ref_ = &nref;
        conn_ = nref.onDestruction([=]{
            ref_ = nullptr;
            conn_.destroy();
        });
    }
    void reset()
    {
        if(!ref_) return;

        conn_.destroy();
        ref_ = nullptr;
    }
};

}
