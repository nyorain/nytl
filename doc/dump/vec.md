nytl::Vec vs std::array
-----------------------

std::array is thought as a replacement for plain arrays where they are not that
well to handle (like e.g. when passing to/returning from a function). They are just some
plain container without special meaning.

nytl::Vec on the the other side is closer to some mathematical understanding of a vector.


template<typename T, std::size_t... Sizes>
struct StackStorage
{
	std::array<mult(Sizes...), T> storage_;

	template<typename... Args>
	decltype(auto) at(VariadicDummy<Args...> pos)
	{
		return (pos * Sizes) + ...;
	}
};

using static22ui = Table<unsigned int, StackStorage, 2, 2>;
