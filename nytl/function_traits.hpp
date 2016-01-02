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

///\file
///\brief Defines utility templates to get information about callable types.

#pragma once

#include <tuple>

namespace nytl
{

namespace detail
{

template<typename T>
struct isCallableImpl
{
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback
    {
        void operator()();
    };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;
    template<typename> static yes test(...);
    template<typename C> static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

}

template<typename T> using is_callable = typename std::conditional<std::is_class<T>::value, detail::isCallableImpl<T>, std::false_type>::type;


//base for every functor or class
template<typename F> class function_traits;

//for raw function signature
template<typename Ret, typename... Args>
class function_traits<Ret(Args...)>
{
public:
    using arg_tuple = std::tuple<Args...>;

protected:
    template<size_t i> struct argImpl
    {
        using type = typename std::tuple_element<i, arg_tuple>::type;
    };

public:
    using return_type = Ret;
    template<size_t i> using arg_type = typename argImpl<i>::type;
    constexpr static const size_t arg_size = std::tuple_size<arg_tuple>::value;
};

//function pointer
template<typename Ret, typename... Args>
class function_traits<Ret(*)(Args...)> : public function_traits<Ret(Args...)>
{
};

//member function pointer
template<typename C, typename Ret, typename... Args>
class function_traits<Ret(C::*)(Args...)> : public function_traits<Ret(Args...)>
{
};

//const member function pointer
template<typename C, typename Ret, typename... Args>
class function_traits<Ret(C::*)(Args...) const> : public function_traits<Ret(Args...)>
{
};

//functor, class
template<typename F> class function_traits : public function_traits<decltype(&F::operator())>
{
};

template<typename F>
class function_traits<F&> : public function_traits<F>
{
};

template<typename F>
class function_traits<F&&> : public function_traits<F>
{
};

}
