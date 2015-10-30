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

#include <nytl/make_unique.hpp>
#include <memory>
#include <type_traits>

namespace nytl
{

//derive cloneable
template<typename Base, typename Derived> class deriveCloneable : public Base
{
protected:
    constexpr deriveCloneable() noexcept
    {
        static_assert(std::is_copy_constructible<Derived>::value, 
				"Your class must be copy constructible");
    }

public:
    virtual std::unique_ptr<Base> clone() const override
		{ return make_unique<Derived>(*(static_cast<const Derived*>(this))); } 
};


/* useful?
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
*/
}
