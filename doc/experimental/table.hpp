#include <cstdint>

template<typename T, std::size_t... Sizes>
struct DimArray;

template<typename T, std::size_t Head, std::size_t... Tail>
struct DimArray<T, Head, Tail...>
{
	DimArray<T, Tail...> data_[Head];

	template<typename... Args> constexpr 
	decltype(auto) at(std::size_t i, Args... args) { return data_[i].at(args...); }

	template<typename... Args> constexpr 
	decltype(auto) at(std::size_t i, Args... args) const { return data_[i].at(args...); }
};

template<typename T, std::size_t Head>
struct DimArray<T, Head>
{
	T data_[Head] {};

	constexpr T& at(std::size_t i) { return data_[i]; }
	constexpr const T& at(std::size_t i) const { return data_[i]; }
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

	template<typename... Args>  constexpr
	decltype(auto) at(Args... args) { return data_.at(args...); }

	template<typename... Args>  constexpr
	decltype(auto) at(Args... args) const { return data_.at(args...); }
};


//compile testing
void func()
{
	constexpr Table<int, 4, 3, 2, 6> myTable {};
	constexpr int val = myTable.at(0, 0, 0, 4);
	static_assert(val == 0, "Wrong");
}
