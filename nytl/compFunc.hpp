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
///\brief Defines the compatibleFunction (compFunc typedef'd) template class.

#pragma once

#include <nytl/function_traits.hpp>

#include <nytl/bits/tupleMap.inl>
#include <nytl/bits/apply.inl>

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

namespace nytl
{

//Not specified, use the function-signature template <Ret(Args...)> like std::function.
template<typename> class compatibleFunction;

///\brief A Function object that is able to hold all functions with a compatible signature.
///\ingroup function
template<typename Ret, typename... Args>
class compatibleFunction<Ret(Args...)>
{
public:
	using func_type = std::function<Ret(Args...)>;
	using comp_func_type = compatibleFunction<Ret(Args...)>;

protected:
    func_type func_ {};

public:
    compatibleFunction() = default;
    ~compatibleFunction() = default;

    //constructor
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction(F func) noexcept { set(func); }

    compatibleFunction(const comp_func_type& other) noexcept 
		: func_(other.func_) {}
    template<typename Sig> compatibleFunction(const compatibleFunction<Sig>& other) noexcept 
		{ set(other.func_); }

    //assignement
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    comp_func_type& operator=(F func) noexcept { set(func); return *this; }

    comp_func_type& operator=(const comp_func_type& other) noexcept 
		{ func_ = other.func_; return *this; }
    template<typename Sig> comp_func_type& operator=(const compatibleFunction<Sig>& other) noexcept
		{ set(other.func_); return *this; }

    //set
    template<typename F> void set(F func) noexcept
    {
        using orgArgsT = std::tuple<Args...>;
        using newTraits = function_traits<F>;
        using newArgsT = typename newTraits::arg_tuple;
        using newRet = typename newTraits::return_type;
        using mapT = detail::tupleMap<orgArgsT, newArgsT>;

        static_assert(std::is_convertible<Ret, newRet>::value, "Return types not compatible");
        static_assert(mapT::seq::size() == newTraits::arg_size, "Arguments not compatible");

        func_ = [=](Args&&... args) -> Ret {
                return static_cast<Ret>(apply(func, mapT::map(std::forward<Args>(args)...)));
            };
    }

    //get
    func_type function() const noexcept { return func_; }

    //call
    Ret call(Args... args) const { return func_(args...); }
    Ret operator()(Args... args) const { return func_(args...); }

	operator bool() const { return function(); }
};

template<typename... Args>
class compatibleFunction<void(Args...)>
{
public:
	using func_type = std::function<void(Args...)>;
	using comp_func_type = compatibleFunction<void(Args...)>;

protected:
	func_type func_;

public:
    compatibleFunction() = default;
    ~compatibleFunction() = default;

    //constructor
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction(F func) noexcept { set(func); }

    compatibleFunction(const comp_func_type& other) noexcept 
		: func_(other.func_) {}
    template<typename Sig> compatibleFunction(const compatibleFunction<Sig>& other) noexcept 
		{ set(other.func_); }

    //assignement
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    comp_func_type& operator=(F func) noexcept { set(func); return *this; }

    comp_func_type& operator=(const comp_func_type& other) noexcept 
		{ func_ = other.func_; return *this; }
    template<typename Sig> comp_func_type& operator=(const compatibleFunction<Sig>& other) noexcept
		{ set(other.func_); return *this; }

    //set
    template<typename F>
    void set(F func) noexcept
    {
        using orgArgsT = std::tuple<Args...>;
        using newTraits = function_traits<F>;
        using newArgsT = typename newTraits::arg_tuple;
        using mapT = detail::tupleMap<orgArgsT, newArgsT>;

        static_assert(mapT::seq::size() == newTraits::arg_size, "Arguments not compatible");

        func_ = [=](Args&&... args) -> void {
                apply(func, mapT::map(std::forward<Args>(args)...));
            };
    }

    //get
    func_type function() const noexcept { return func_; }

    //call
    void call(Args... args) const { func_(args...); }
    void operator()(Args... args) const { func_(args...); }

	operator bool() const { return function(); }
};

//typedef compFunc
template<typename Signature> using compFunc = compatibleFunction<Signature>;


}
