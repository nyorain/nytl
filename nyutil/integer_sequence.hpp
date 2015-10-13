#pragma once

#include <nyutil/tmp.hpp>
#include <utility>

namespace nyutil
{

#if __cplusplus >= 201402L
 using std::integer_sequence;
 using std::index_sequence;
 using std::make_index_sequence;
 using std::make_integer_sequence;
#else

template<typename T, T... Ints> class integer_sequence
{
public:
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

template<std::size_t... Ints> using index_sequence = integer_sequence<std::size_t, Ints...>;

//detail
namespace detail
{

template<typename T, std::size_t highest> class make_integer_sequenceT
{
public:
    using type = seq_append<typename make_integer_sequenceT<T, highest - 1>::type, highest>;
};

template<typename T> class make_integer_sequenceT<T, 0>
{
public:
    using type = index_sequence<0>;
};

}

//make typedefs
template<typename T, std::size_t highest> using make_integer_sequence = typename detail::make_integer_sequenceT<T, highest>::type;
template<std::size_t highest> using make_index_sequence = make_integer_sequence<std::size_t, highest>;

#endif // __cplusplus
}
