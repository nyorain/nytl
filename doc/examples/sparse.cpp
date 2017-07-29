#include <nytl/matOps.hpp>
#include <nytl/assure.hpp>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <array>

/// The example classes do not implement all needed operations to fully fulfill
/// their nytl concept counterparts. Only the needed operations were implementated
/// due to lazyness.

/// Base class for sparse implementations.
/// Simply holds a 1-dimensional positional lookup table and allows to
/// access it.
template<typename T>
class SparseBase {
public:
	using Size = std::size_t;
	using Value = T;
	using Reference = T&;
	using ConstReference = const T&;

	static constexpr bool staticSized = false;

public:
	T get(Size i) const
	{
		auto e = entries_.find(i);
		return e == entries_.end() ? 0 : e->second;
	}
	void set(Size i, const T& val)
	{
		auto e = entries_.find(i);
		if(val == 0) {
			if(e != entries_.end()) entries_.erase(e);
			return;
		}

		if(e != entries_.end()) e->second = val;
		else entries_.emplace(i, val);
	}

	auto& entries() { return entries_; }
	auto& entries() const { return entries_; }

protected:
	std::unordered_map<Size, T> entries_;
};

/// Example sparse vector class.
/// Fulfills the nytl vector concept.
template<typename T>
class SparseVec : public SparseBase<T> {
public:
	using typename SparseBase<T>::Size;

	template<typename O> using Rebind = SparseVec<O>;
	static SparseVec create(Size i) { return {i}; }

public:
	SparseVec(Size i = 0) : size_(i) {}
	Size size() const { return size_; }

	T at(Size i) const {
		if(i >= size())
			throw std::out_of_range("SparseVec::at");
		return get(i);
	}

protected:
	Size size_;
};

/// Example sparse matrix class.
/// Fulfils the nytl matrix concept.
template<typename T>
class SparseMat : SparseBase<T> {
public:
	using typename SparseBase<T>::Size;
	using typename SparseBase<T>::Value;
	using typename SparseBase<T>::Reference;
	using typename SparseBase<T>::ConstReference;

	using RowVec = SparseVec<T>;
	using ColVec = SparseVec<T>;

	static constexpr bool staticSized = false;

	template<typename O> using Rebind = SparseMat<O>;
	static SparseMat create(Size r, Size c) { return {r, c}; }

public:
	SparseMat(Size r = 0, Size c = 0) : rows_(r), cols_(c) {}

	T get(Size r, Size c) const { return SparseBase<T>::get(r * cols_ + c); }
	void set(Size r, Size c, const T& val) { SparseBase<T>::set(r * cols_ + c, val); }
	void size(Size r, Size c) { rows_ = r; cols_ = c; }

	T at(Size r, Size c) const {
		if(r >= rows_ || c >= cols_)
			throw std::out_of_range("SparseMatrix::at");
		return get(r, c);
	}

	Size rows() const { return rows_; }
	Size cols() const { return cols_; }

protected:
	std::size_t rows_;
	std::size_t cols_;
};

// - operators -
// vec
template<typename F, typename T>
constexpr auto operator*(const F& f, SparseVec<T> a)
{
	for(auto& e : a.entries())
		e.second *= f;
	return a;
}

template<typename T1, typename T2>
constexpr auto operator+(const SparseVec<T1>& a, const SparseVec<T2>& b)
{
	nytl_assure(false, a.size() == b.size(), "vectors must have same dimension");
	auto ret = SparseVec<decltype(a.get(0) + b.get(0))>::create(a.size());

	for(auto& e : a.entries())
		ret.set(e.first, e.second);

	for(auto& e : b.entries())
		ret.set(e.first, ret.get(e.first) + e.second);

	return ret;
}

template<typename T1, typename T2>
constexpr auto operator-(const SparseVec<T1>& a, const SparseVec<T2>& b)
{
	nytl_assure(false, a.size() == b.size(), "vectors must have same dimension");
	auto ret = SparseVec<decltype(a.get(0) + b.get(0))>::create(a.size());

	for(auto& e : a.entries())
		ret.set(e.first, e.second);

	for(auto& e : b.entries())
		ret.set(e.first, ret.get(e.first) - e.second);

	return ret;
}

// mat
template<typename T1, typename T2>
auto operator*(const SparseMat<T1>& a, const SparseMat<T2>& b)
{
	// NOTE: This implementation is not optimized
	nytl_assure(false, a.cols() == b.rows(), "Invalid dimensions for matrix mult");

	using Type = decltype(a.get(0, 0) * b.get(0, 0) + a.get(0, 0) * b.get(0, 0));
	auto ret = SparseMat<Type>::create(a.rows(), b.cols());

	for(auto r = 0u; r < ret.rows(); ++r)
		for(auto c = 0u; c < ret.cols(); ++c)
			ret.set(r, c, nytl::vec::dot(nytl::mat::row(a, r), nytl::mat::col(b, c)));

	return ret;
}

template<typename T1, typename T2>
bool operator==(const SparseMat<T1>& a, const SparseMat<T2>& b)
{
	if(a.rows() != b.rows() || a.cols() != b.cols())
		return false;

	for(auto r = 0u; r < a.rows(); ++r)
		for(auto c = 0u; c < a.cols(); ++c)
			if(a.get(r, c) != b.get(r, c))
				return false;

	return true;
}

int main()
{
	// just some test/example operations

	{
		SparseVec<double> a {5};
		SparseVec<double> b {5};
		a.set(2, 3);
		a.set(3, 5);
		b.set(1, -3);
		b.set(3, 5);
		b.set(4, 1);

		std::cout << "vec:\n";
		nytl::vec::print(std::cout, a);
		std::cout << "\n";
		nytl::vec::print(std::cout, b);
		std::cout << "\n";
		nytl::vec::print(std::cout, a + b);
		std::cout << "\n";
		nytl::vec::print(std::cout, a - b);
		std::cout << "\n";
		nytl::vec::print(std::cout, a - 2 * b);
		std::cout << "\n";
	}

	{
		std::cout << "\nmat:\n";
		SparseMat<double> a {3, 3};

		nytl::mat::identity(a);
		a.set(0, 2, 7);

		nytl::mat::print(std::cout, a, 2);

		// assert(nytl::mat::trace(a) == 10.0);
		// assert(nytl::mat::multiplyDiagonal(a) == 1.0);
		// assert(nytl::mat::symmetric(a));

		auto b = nytl::mat::inverse(a);
		nytl::mat::print(std::cout, a, 2);
		nytl::mat::print(std::cout, b, 2);

		auto mult = a * b;
		auto id = mult;
		std::cout << "\na * inverse(a): : should be identity:\n";
		nytl::mat::print(std::cout, mult, 2);
		nytl::mat::identity(id);
		assert(mult == id);
	}
}

#undef nytl_assure
