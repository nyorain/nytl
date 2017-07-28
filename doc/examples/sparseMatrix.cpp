#include <nytl/matOps.hpp>
#include <array>

/// Example class for a matrix that only has values on its diagonal
/// Static sized.
template<std::size_t I, typename T>
class DiagonalMatrix {
public:
	using Size = std::size_t;
	using Value = T;

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

/// Example class for a matrix that gets values set dynamically.
/// Note that this is often (e.g. for small matrices) not cheaper
/// than the nytl::Mat template since this uses the heap.
/// For simplicity purposes, uses no hashtable (or comparable) but
/// simply a vector.
/// Dynamic sized.
template<typename T>
class SparseMatrix {
public:
	using Size = std::size_t;
	using Value = T;

	template<typename O>
	using Rebind = SparseMatrix<O>;

public:
	T get(Size r, Size c) const
	{
		auto e = find(r, c);
		return e == entries_.end() ? 0 : e->value;
	}
	T at(Size r, Size c) const
	{
		if(r >= rows() || c >= cols())
			throw std::out_of_range("SparseMatrix::at");

		auto e = find(r, c);
		return e == entries_.end() ? 0 : e->value;
	}
	void set(Size r, Size c, const T& val)
	{
		auto e = find(r, c);
		if(val == 0) {
			if(e != entries_.end()) entries_.erase(e);
			return;
		}

		if(e != entries_.end()) e->value = val;
		else entries_.push_back({r, c, val});
	}

	void resize(Size r, Size c)
	{
		rows_ = r;
		cols_ = c;
	}

	Size rows() const { return rows_; }
	Size cols() const { return cols_; }

protected:
	struct Entry {
		std::size_t row;
		std::size_t col;
		T value;
	};

	std::size_t rows_;
	std::size_t cols_;
	std::vector<Entry> entries_;

	auto find(Size r, Size c)
	{
		return std::find_if(entries_, [&](auto& e){ return e.row == r && e.col == c; });
	}
};

int main()
{
	constexpr DiagonalMatrix<5, double> a;
	static_assert(nytl::mat::symmetric(a));
	static_assert(a.get(0, 0) == 0.0);
	static_assert(a.get(1, 0) == 0.0);

	DiagonalMatrix<20, double> b;
	b.set(5, 5, 5.0); // ok
	b.set(5, 0, 1.0); // error
}
