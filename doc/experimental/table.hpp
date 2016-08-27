#include <cstdint>
#include <type_traits>
#include <iterator>
#include <utility>
#include <functional>

#include <nytl/tmp.hpp>
using namespace nytl;

using Expand = int[];
template<typename... Args> constexpr std::size_t mult(Args&&... args)
{
	return (args * ...);
}

template<typename T, std::size_t... Sizes>
struct DimArray;

template<typename T, std::size_t Head, std::size_t... Tail>
struct DimArray<T, Head, Tail...>
{
	DimArray<T, Tail...> data_[Head];

	// template<typename It, std::size_t... I>
	// DimArray(It it, std::index_sequence<I...>) : data_(it, ) {}

	constexpr DimArray() = default;

	template<typename... Args> constexpr
	decltype(auto) at(std::size_t i, Args... args) { return data_[i].at(args...); }

	template<typename... Args> constexpr
	decltype(auto) at(std::size_t i, Args... args) const { return data_[i].at(args...); }

	constexpr T* begin() { return data_[0].begin(); }
	constexpr T* end() { return data_[Head - 1].end(); } //just data_[Head]?

	constexpr const T* begin() const { return data_[0].begin(); }
	constexpr const T* end() const { return data_[Head - 1].end(); }
};

template<typename T, std::size_t Head>
struct DimArray<T, Head>
{
	T data_[Head] {};

	template<typename It, std::size_t... I> constexpr
	DimArray(It it, std::index_sequence<I...>) : data_{it[I]...} {}

	template<typename... Args, typename = std::enable_if_t<
			std::is_convertible<std::tuple<Args...>, TypeTuple<T, Head>>::value>>
	constexpr DimArray(Args&&... args) noexcept : data_{std::forward<Args>(args)...} {}

	constexpr DimArray() = default;

	constexpr T& at(std::size_t i) { return data_[i]; }
	constexpr const T& at(std::size_t i) const { return data_[i]; }

	constexpr T* begin() { return std::begin(data_); }
	constexpr T* end() { return std::end(data_); }

	constexpr const T* begin() const { return std::begin(data_); }
	constexpr const T* end() const { return std::end(data_); }
};

///Abstracts the concept of a vector or matrix to N dimensions.
///N is defined as sizeof...(Sizes).
///Basically just a table of numbers with N dimensions and the given Sizes.
///One can pass nytl::dynamicSize as dimension size.
template<typename P, std::size_t... Sizes>
class Table
{
public:
	// Behaves as (memory layout as):
	// P data_[Sizes[0]][Sizes[1]][Sizes[2]][...][Sizes[sizeof...(Sizes) - 1]];
	DimArray<P, Sizes...> data_;

public:
	constexpr Table() noexcept = default;

	template<typename It>
	constexpr Table(It begin, It end)
	{ }

	template<typename... Args>  constexpr
	decltype(auto) at(Args... args) { return data_.at(args...); }

	template<typename... Args>  constexpr
	decltype(auto) at(Args... args) const { return data_.at(args...); }

	constexpr auto begin() { return data_.begin(); }
	constexpr auto end() { return data_.end(); }

	constexpr auto begin() const { return data_.begin(); }
	constexpr auto end() const { return data_.end(); }
};

///One can defined all sorts of mathematic operations over a table.
///Note that multiplication is not clealy defined for a table and therefore no general operator
///overload is used, just different functions

///Tensor (or outer) product.
template<typename P1, typename P2, std::size_t... S1, std::size_t... S2> constexpr
auto tensor(const Table<P1, S1...>& t1, const Table<P2, S2...>& t2)
{
	using RetVal = decltype(std::declval<P1>() * std::declval<P2>());
	return Table<RetVal, S1..., S2...>{};
}

///inner product?

///collapse the table into a bigger one with fewer dimensions.
template<typename P, std::size_t... S, typename PO, std::size_t... SO,
	typename = std::enable_if_t<mult(SO...) == mult(S...)>> constexpr
Table<P, S...> collapse(const Table<PO, SO...>& t1)
{
	// return reinterpret_cast<const Table<P, S...>&>(t1);
	// return Table<P, S...>(t1.begin(), t1.end());
	return {};
}


//compile testing
void func()
{
	constexpr Table<int, 4, 3> myTable {};
	constexpr int val = myTable.at(0, 2);
	static_assert(val == 0, "Wrong");

	// constexpr DimArray<int, 1> da(myTable.begin(), std::make_index_sequence<0>{});
	constexpr DimArray<int, 3> da(5, 7, 3);
	static_assert(da.data_[0] == 5, "Wrong");

	constexpr auto t2 = collapse<int, 12>(myTable);
	// constexpr auto t3 = tensor(myTable, myTable);
}
