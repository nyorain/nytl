#include <nytl/matOps.hpp>
#include <cstddef>
#include <iostream>
#include <array>
#include <cassert>

/// Example class for a matrix that only has values on its diagonal
/// Does not fulfill the Mat specification in all traits (it cannot do so by
/// design) and can therefore not be used for operations like inverting it
/// or bringing it into row echolon form, but since the structure of the matrix
/// is known it should not be needed.
/// Static sized.
template<std::size_t I, typename T>
class DiagonalMatrix {
public:
	using Size = std::size_t;
	using Value = T;

	// Cannot provide those, produce compile-time error if accessed
	// using RowVec = nope;
	// using ColVec = nope;

	template<typename O> using Rebind = DiagonalMatrix<I, O>;
	static constexpr bool staticSized = true;

	static constexpr Size rows() { return I; }
	static constexpr Size cols() { return I; }
	template<Size R, Size C> static DiagonalMatrix<R, T> create() {
		static_assert(R == C, "DiagonalMatrix must be quadratic");
		return {};
	}

public:
	constexpr T get(Size r, Size c) const { return (r == c) ? values[r] : 0; }
	constexpr T at(Size r, Size c) const
	{
		if(r >= I || c >= I) throw std::out_of_range("DiagonalMatrix::at");
		return (r == c) ? values.at[r] : 0;
	}

	constexpr T& set(Size r, Size c, const T& val)
	{
		if(r != c) throw std::logic_error("DiagonalMatrix::set: not on diagonal");
		return (values[r] = val);
	}

protected:
	std::array<T, I> values {};
};

// To fulfill the matrix concept at least as far as possible here should follow
// operator implementations for the matrix value.

int main()
{
	constexpr DiagonalMatrix<5, double> a;
	static_assert(nytl::mat::symmetric(a));
	static_assert(nytl::mat::trace(a) == 0);
	static_assert(nytl::mat::multiplyDiagonal(a) == 0);

	static_assert(a.get(0, 0) == 0.0);
	static_assert(a.get(1, 0) == 0.0);

	DiagonalMatrix<10, double> b;
	b.set(5, 5, 5.0); // ok
	// b.set(5, 0, 1.0); // runtime error

	assert(nytl::mat::trace(b) == 5.0);
	assert(nytl::mat::multiplyDiagonal(b) == 0);
	nytl::mat::print(std::cout, b);
}
