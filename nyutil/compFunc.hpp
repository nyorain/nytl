#pragma once

#include <nyutil/function_traits.hpp>
#include <nyutil/tmp.hpp>

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

namespace nyutil
{

//todo: use tmp nyutil header

namespace tmp
{

//eraseFirst
template<typename tup> struct eraseFirst;

template<typename Head, typename ... Tail>
struct eraseFirst<std::tuple<Head, Tail...>>
{
    using type = std::tuple<Tail...>;
};

//seqPrepend
template<typename seq, size_t prep> struct seqPrepend;

template<size_t... idx, size_t prep>
struct seqPrepend<std::index_sequence<idx...>, prep>
{
    using type = std::index_sequence<prep, idx...>;
};

//seqAppend
//seqPrepend
template<typename seq, size_t ap> struct seqAppend;

template<size_t... idx, size_t ap>
struct seqAppend<std::index_sequence<idx...>, ap>
{
    using type = std::index_sequence<idx..., ap>;
};

//sequenceSize
template<typename seq> struct seqSize;

template<size_t... idx>
struct seqSize<std::index_sequence<idx...>>
{
    constexpr static const size_t value = sizeof...(idx);
};


}


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
        typename tmp::seqPrepend< //matches
            typename tupleMapImpl<
                typename tmp::eraseFirst<orgTuple>::type,
                typename tmp::eraseFirst<newTuple>::type,
                idx + 1
            >::type,
            idx
        >::type,
        typename tupleMapImpl< //dont matches
            typename tmp::eraseFirst<orgTuple>::type,
            newTuple,
            idx + 1
        >::type
    >::type type;
};

template<typename... OrgLeft, size_t idx>
struct tupleMapImpl<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
    using type = std::index_sequence<>;
};

template<size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<>, idx>
{
    using type = std::index_sequence<>;
};

template<typename... NewLeft, size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<NewLeft...>, idx>
{
    using type = std::index_sequence<>;

    //ERROR. Should NEVER happen!
    //how to show error if this is used, but only IF it is used (std::confitional must still work)?
};

}


//map
template<typename orgTup, typename newTup, typename seq = typename detail::tupleMapImpl<orgTup, newTup>::type> struct tupleMap;

template<typename... orgArgs, typename... newArgs, size_t... idx>
struct tupleMap<std::tuple<orgArgs...>, std::tuple<newArgs...>, std::index_sequence<idx...>>
{
    using newTup = std::tuple<newArgs...>;
    using orgTup = std::tuple<orgArgs...>;
    using seq = std::index_sequence<idx...>;

    static constexpr newTup map(orgArgs&&... args) noexcept
    {
        orgTup org(std::forward<orgArgs>(args)...);
        newTup ret(std::forward<decltype(std::get<idx>(org))>(std::get<idx>(org))...);

        return ret;
    }
};


//experimental::tuple::apply example implementation
//http://en.cppreference.com/w/cpp/experimental/apply
template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) apply_impl( F&& f, Tuple&& t, std::index_sequence<I...> )
{
    //return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    return f(std::get<I>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t)
{
    return apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>{});
}



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
        static_assert(tmp::seqSize<typename mapT::seq>::value == newTraits::arg_size, "Arguments are not compatible");

        func_ = [=](ArgsOrg&&... args) -> RetOrg {
                return static_cast<RetOrg>(apply(func, mapT::map(std::forward<ArgsOrg>(args)...)));
            };
    }

    //get
    std::function<RetOrg(ArgsOrg...)> function() const noexcept { return func_; }

    //call
    RetOrg call(ArgsOrg... args) const { return func_(args...); }
    //RetOrg operator()(ArgsOrg... args) const { return func_(args...); }
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

        static_assert(tmp::seqSize<typename mapT::seq>::value == newTraits::arg_size, "Your function arguments are not compatible");

        func_ = [=](ArgsOrg&&... args) -> void {
                apply(func, mapT::map(std::forward<ArgsOrg>(args)...));
            };
    }

    //get
    std::function<void(ArgsOrg...)> function() const noexcept { return func_; }

    //call
    void call(ArgsOrg... args) const { func_(args...); }
    //void operator()(ArgsOrg... args) const { func_(args...); }
};

//typedef compFunc
template<typename Signature> using compFunc = compatibleFunction<Signature>;


}
