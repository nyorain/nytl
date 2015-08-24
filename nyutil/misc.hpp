#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace nyutil
{

template<class... T> void unused(T&&...)
{ }

template<class U, class V, class ...t> std::function<U(t...)> memberCallback(U (V::*func)(t ...), V* obj)
{
    return ([=](t ... params)
    {
        return (obj->*func)(params ...);
    });
}

template<class U, class V, class ...t> std::function<U(t...)> memberCallback(U (V::*func)(t ...), V& obj)
{
    return std::bind(func, &obj);
}

template<class U, class ... t, class ... o> std::function<U(t..., o...)> addParameters(std::function<U(t ...)> func)
{
    return ([func](t ... params, o ... unused) -> U
    {
        return (func(params ...));
    });
}


template<class ... Args> void printVars(std::ostream& out, Args ... args)
{
    int a[sizeof...(Args)] = { (out << args, 0)... };
    unused(a);
}


//credit thttp:/stackoverflow.com/users/13430/evan-teran
inline std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//dumpVector
template<class T> std::string dumpVector(const std::vector<T>& obj)
{
    const char* c = "";
    std::stringstream ss;

    ss << "(";

    for(unsigned int i(0); i < obj.size(); i++)
    {
        ss << c << obj[i];
        c = ",";
    }

    ss << ")";

    return ss.str();
}

//copyVector
template<class A, class B> void copyVector(const std::vector<A>& a, std::vector<B>& b)
{
    for(unsigned int i(0); i < a.size(); i++)
    {
        const B tmp = (B) a[i];
        b.push_back(tmp);
    }
}

template<class A, class B> std::vector<B> copyVector(const std::vector<A>& a)
{
    std::vector<B> ret;
    copyVector(a, ret);
    return ret;
}



template<class A, class B> std::vector<B> copyVectorLike(const A& a)
{
    std::vector<B> ret;
    for(unsigned int i(0); i < a.size(); i++)
    {
        const B tmp = (B) a[i];
        ret.push_back(tmp);
    }
    return ret;
}


//functions
inline double absDistance(double x1, double y1, double x2, double y2){ return sqrt(pow(x2 - x1,2) + pow(y2 - y1,2)); }

template <class T> bool contains(std::vector<T> vec, T val)
{
    return(std::find(vec.begin(), vec.end(), val) != vec.end());
};

inline float randomFloat(float low, float high)
{
    return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

inline int randomInt(int low, int high)
{
    return (int) low + rand() % (high - low);
}

//compFunc
//experimental::tuple::apply
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

//match
template<typename A, typename B, typename Cond = void>
struct match : std::false_type
{
};

template<typename A, typename B>
struct match<A, B, typename std::enable_if<std::is_convertible<A, B>::value>::type> : public std::true_type {};

//find
template<typename orgTup, typename newTup, size_t idx = 0> struct findMap;

template<typename... orgArgs, typename... newArgs, size_t idx>
struct findMap<std::tuple<orgArgs...>, std::tuple<newArgs...>, idx>
{
    using orgTuple = std::tuple<orgArgs...>;
    using newTuple = std::tuple<newArgs...>;

    constexpr static const bool value = match<typename std::tuple_element<0, orgTuple>::type, typename std::tuple_element<0, newTuple>::type>::value;
    typedef typename std::conditional<
        value,  //condition
        typename seqPrepend< //matches
            typename findMap<
                typename eraseFirst<orgTuple>::type,
                typename eraseFirst<newTuple>::type,
                idx + 1
            >::type,
            idx
        >::type,
        typename findMap< //dont matches
            typename eraseFirst<orgTuple>::type,
            newTuple,
            idx + 1
        >::type
    >::type type;
};

template<typename... OrgLeft, size_t idx>
struct findMap<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
    using type = std::index_sequence<>;
};

template<size_t idx>
struct findMap<std::tuple<>, std::tuple<>, idx>
{
    using type = std::index_sequence<>;
};

template<typename... NewLeft, size_t idx>
struct findMap<std::tuple<>, std::tuple<NewLeft...>, idx>
{
    using type = std::index_sequence<>;

    //ERROR. Should NEVER happen!
    //how to show error if this is used, but only IF it is used (std::confitional must still work)?
};

//map
template<typename orgTup, typename newTup, typename seq = typename findMap<orgTup, newTup>::type> struct mapTuple;

template<typename... orgArgs, typename... newArgs, size_t... idx>
struct mapTuple<std::tuple<orgArgs...>, std::tuple<newArgs...>, std::index_sequence<idx...>>
{
    using newTup = std::tuple<newArgs...>;
    using orgTup = std::tuple<orgArgs...>;
    using seq = std::index_sequence<idx...>;

    newTup map(orgArgs... args) const noexcept
    {
        orgTup org(args...);
        newTup ret(std::get<idx>(org)...);

        return ret;
    }
};

//sequenceSize
template<typename seq> struct sequenceSize;

template<size_t... idx>
struct sequenceSize<std::index_sequence<idx...>>
{
    constexpr static const size_t value = sizeof...(idx);
};


//compFunc
//todo: void spec
template<typename> class compatibleFunction;

//general
template<typename RetOrg, typename... ArgsOrg>
class compatibleFunction<RetOrg(ArgsOrg...)>
{
protected:
    std::function<RetOrg(ArgsOrg...)> func_ {};

public:
    template<typename RetNew, typename... ArgsNew>
    compatibleFunction(std::function<RetNew(ArgsNew...)> func) noexcept
    {
        using orgArgsT = std::tuple<ArgsOrg...>;
        using newArgsT = std::tuple<ArgsNew...>;
        using mapT = mapTuple<orgArgsT, newArgsT>;

        static_assert(std::is_convertible<RetOrg, RetNew>::value, "Return types are not compatible");
        static_assert(sequenceSize<typename mapT::seq>::value == sizeof...(ArgsNew) , "Arguments are not compatible");

        mapT argMap;

        func_ = [=](ArgsOrg... args) -> RetOrg {
                return static_cast<RetOrg>(apply(func, argMap.map(args...)));
            };
    }

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
    template<typename RetNew, typename... ArgsNew>
    compatibleFunction(std::function<RetNew(ArgsNew...)> func) noexcept
    {
        using orgArgsT = std::tuple<ArgsOrg...>;
        using newArgsT = std::tuple<ArgsNew...>;
        using mapT = mapTuple<orgArgsT, newArgsT>;

        static_assert(sequenceSize<typename mapT::seq>::value == sizeof...(ArgsNew) , "Arguments are not compatible");
        mapT argMap;

        func_ = [=](ArgsOrg... args) -> void {
                apply(func, argMap.map(args...));
            };
    }

    void call(ArgsOrg... args) const { func_(args...); }
    void operator()(ArgsOrg... args) const { func_(args...); }
};

template<typename Sig> using compFunc = compatibleFunction<Sig>;

}

