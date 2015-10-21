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

#include <nyutil/function_traits.hpp>
#include <nyutil/tmp.hpp>
#include <nyutil/integer_sequence.hpp>

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

#if __cplusplus >= 201402L
#include <experimental/tuple>
#endif //c++14, todo: is this header c++14 standard?

namespace nyutil
{


//tupleMap
namespace detail
{

template<typename orgTup, typename newTup, size_t idx = 0> struct tupleMapImpl;

template<typename... orgArgs, typename... newArgs, size_t idx>
struct tupleMapImpl<std::tuple<orgArgs...>, std::tuple<newArgs...>, idx>
{
    using orgTuple = std::tuple<orgArgs...>;
    using newTuple = std::tuple<newArgs...>;

    constexpr static const bool value = std::is_convertible<typename std::tuple_element<0, orgTuple>::type, typename std::tuple_element<0, newTuple>::type>::value;
    typedef typename std::conditional<
        value,  //condition
        typename seq_prepend< //match
            typename tupleMapImpl<
                typename tuple_erase_first<orgTuple>::type,
                typename tuple_erase_first<newTuple>::type,
                idx + 1
            >::type,
            idx
        >::type,
        typename tupleMapImpl< //no match
            typename tuple_erase_first<orgTuple>::type,
            newTuple,
            idx + 1
        >::type
    >::type type;
};

template<typename... OrgLeft, size_t idx>
struct tupleMapImpl<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<typename... NewLeft, size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<NewLeft...>, idx>
{
    using type = index_sequence<>;

    //ERROR. Should NEVER happen!
    //how to show error if this is used, but only IF it is used (std::conditional must still work)?
};

}


//map
template<typename orgTup, typename newTup, typename seq = typename detail::tupleMapImpl<orgTup, newTup>::type> struct tupleMap;

template<typename... orgArgs, typename... newArgs, size_t... idx>
struct tupleMap<std::tuple<orgArgs...>, std::tuple<newArgs...>, index_sequence<idx...>>
{
    using newTup = typename std::tuple<newArgs...>;
    using orgTup = typename std::tuple<orgArgs...>;
    using seq = index_sequence<idx...>;

    static constexpr std::tuple<newArgs...> map(orgArgs&&... args) noexcept
    {
        return std::tuple<newArgs...>(std::forward<decltype(std::get<idx>(orgTup(args...)))>(std::get<idx>(orgTup(args...)))...);
    }
};


#if __cplusplus >= 201402L
using std::experimental::apply;
#else
//experimental::tuple::apply example implementation
//http://en.cppreference.com/w/cpp/experimental/apply
namespace detail
{

template <class F, class Tuple, std::size_t... I>
constexpr auto apply_impl( F&& f, Tuple&& t, index_sequence<I...> ) -> decltype(f(std::get<I>(std::forward<Tuple>(t))...))
{
    //return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    return f(std::get<I>(std::forward<Tuple>(t))...);
}

}

template <class F, class Tuple>
constexpr auto apply(F&& f, Tuple&& t) -> decltype(detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
        make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{}))
{
    return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
        make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{});
}

#endif




//compFunc
template<typename> class compatibleFunction;

//general
template<typename RetOrg, typename... ArgsOrg>
class compatibleFunction<RetOrg(ArgsOrg...)>
{
protected:
    std::function<RetOrg(ArgsOrg...)> func_ {};

public:
    compatibleFunction() = default;
    ~compatibleFunction() = default;

    //constructor
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction(F func) noexcept { set(func); }

    compatibleFunction(const compatibleFunction<RetOrg(ArgsOrg...)>& other) noexcept : func_(other.func_) {}
    template<typename Sig> compatibleFunction(const compatibleFunction<Sig>& other) noexcept { set(other.func_); }

    //assignement
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction<RetOrg(ArgsOrg...)>& operator=(F func) noexcept { set(func); return *this; }

    compatibleFunction<RetOrg(ArgsOrg...)>& operator=(const compatibleFunction<RetOrg(ArgsOrg...)>& other) noexcept { func_ = other.func_; return *this; }
    template<typename Sig> compatibleFunction<RetOrg(ArgsOrg...)>& operator=(const compatibleFunction<Sig>& other) noexcept { set(other.func_); return *this; }

    //set
    template<typename F> void set(F func) noexcept
    {
        using orgArgsT = std::tuple<ArgsOrg...>;
        using newTraits = function_traits<F>;
        using newArgsT = typename newTraits::arg_tuple;
        using newRet = typename newTraits::return_type;
        using mapT = tupleMap<orgArgsT, newArgsT>;

        static_assert(std::is_convertible<RetOrg, newRet>::value, "Return types are not compatible");
        static_assert(mapT::seq::size() == newTraits::arg_size, "Arguments are not compatible");

        func_ = [=](ArgsOrg&&... args) -> RetOrg {
                return static_cast<RetOrg>(apply(func, mapT::map(std::forward<ArgsOrg>(args)...)));
            };
    }

    //get
    std::function<RetOrg(ArgsOrg...)> function() const noexcept { return func_; }

    //call
    RetOrg call(ArgsOrg... args) const { return func_(args...); }
    RetOrg operator()(ArgsOrg... args) const { return func_(args...); }
};

//void ret
template<typename... ArgsOrg>
class compatibleFunction<void(ArgsOrg...)>
{
protected:
    std::function<void(ArgsOrg...)> func_ {};

public:
    compatibleFunction() = default;
    ~compatibleFunction() = default;

    //constructor
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction(F func) noexcept { set(func); }

    compatibleFunction(const compatibleFunction<void(ArgsOrg...)>& other) noexcept : func_(other.func_) {}
    template<typename Sig> compatibleFunction(const compatibleFunction<Sig>& other) noexcept { set(other.function()); }

    //assignement
    template<typename F, typename = typename std::enable_if<is_callable<F>::value>::type>
    compatibleFunction<void(ArgsOrg...)>& operator=(F func) noexcept { set(func); return *this; }

    compatibleFunction<void(ArgsOrg...)>& operator=(const compatibleFunction<void(ArgsOrg...)>& other) noexcept { func_ = other.func_; return *this; }
    template<typename Sig> compatibleFunction<void(ArgsOrg...)>& operator=(const compatibleFunction<Sig>& other) noexcept { set(other.function()); return *this; }

    //set
    template<typename F>
    void set(F func) noexcept
    {
        using orgArgsT = std::tuple<ArgsOrg...>;
        using newTraits = function_traits<F>;
        using newArgsT = typename newTraits::arg_tuple;
        using mapT = tupleMap<orgArgsT, newArgsT>;

        static_assert(mapT::seq::size() == newTraits::arg_size, "Your function arguments are not compatible");

        func_ = [=](ArgsOrg&&... args) -> void {
                apply(func, mapT::map(std::forward<ArgsOrg>(args)...));
            };
    }

    //get
    std::function<void(ArgsOrg...)> function() const noexcept { return func_; }

    //call
    void call(ArgsOrg... args) const { func_(args...); }
    void operator()(ArgsOrg... args) const { func_(args...); }
};

//typedef compFunc
template<typename Signature> using compFunc = compatibleFunction<Signature>;


}
