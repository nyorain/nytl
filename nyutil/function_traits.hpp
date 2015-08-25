#pragma once

#include <tuple>

namespace nyutil
{

namespace detail
{

//from SO
//todo: give credit, search link and user [is callable]
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
template<typename F> struct function_traits : public function_traits<decltype(&F::operator())>
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
