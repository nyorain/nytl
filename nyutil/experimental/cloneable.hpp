#pragma once

#include <memory>
#include <type_traits>

namespace nyutil
{

//basic cloneable
template<typename T> class cloneable
{
protected:
    constexpr cloneable() noexcept
    {
        static_assert(std::is_base_of<cloneable<T>, T>::value, "You have to use a derived class as template parameter");
        static_assert(std::is_copy_constructible<T>::value, "Your class must be copy constructible");
    }

public:
    std::unique_ptr<T> clone() const { return std::make_unique<T>(*(static_cast<const T*>(this))); }
    //T cloneOnStack() const { return T(*(static_cast<const T*>(this))); } //needed?
};

//derive cloneable
template<typename Base, typename Derived> class deriveCloneable : public Base
{
protected:
    constexpr deriveCloneable() noexcept
    {
        static_assert(std::is_copy_constructible<Derived>::value, "Your class must be copy constructible");
    }

public:
    std::unique_ptr<Base> clone() const { return std::make_unique<Derived>(*(static_cast<const Derived*>(this))); } //virtual&override?
    //Derived cloneOnStack() const { return Derived(*(static_cast<const Derived*>(this))); } //needed?
};

}
