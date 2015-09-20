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
