// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various matrix related operations.
/// All types in this file named M or M* are expected to fulfil the Matrix
/// concept desribed by doc/mat.md

#pragma once

#ifndef NYTL_INCLUDE_MAT_OPS
#define NYTL_INCLUDE_MAT_OPS

#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/vecOps.hpp>

#include <utility> // std::swap
#include <stdexcept> // std::invalid_argument
#include <tuple> // std::tuple
#include <iosfwd> // std::ostream
#include <cmath>

namespace nytl::mat {
namespace detail {

/// \brief Creates a matrix of implementation type 'M' with value type 'T' and
/// rows/cols 'R'/'C'.
template<typename M, typename T, std::size_t R, std::size_t C>
auto createMatrix()
{
	if constexpr(M::staticSized) return M::template Rebind<T>::template create<R, C>();
	else return M::template Rebind<T>::create(R, C);
}

/// \brief Creates a matrix with the transposed dimension as the given one.
/// Will not copy/set any values.
template<typename M>
auto createTransposeMatrix(const M& mat)
{
	if constexpr(M::staticSized) return M::template create<M::cols(), M::rows()>();
	else return M::create(mat.cols(), mat.rows());
}

/// \brief Creates a matrix with the same implementation and dimension as the given one
/// but with value type 'T'
template<typename T, typename M>
auto createMatrix(const M& mat)
{
	if constexpr(M::staticSized)
		return M::template Rebind<T>::template create<M::rows(), M::cols()>();
	else return M::template Rebind<T>::create(mat.rows(), mat.cols());
}

template<typename M, bool Row>
class MatProxyBase {
public:
	using Value = typename M::Value;
	using Size = typename M::Size;
	using Reference = typename M::Reference;
	using ConstReference = typename M::ConstReference;

	using MatVec = std::conditional_t<Row, typename M::RowVec, typename M::ColVec>;
	template<typename T> using Rebind = typename MatVec::template Rebind<T>;

public:
	M& m;
	Size n;

public:
	constexpr decltype(auto) get(Size i) { return m.get(Row ? n : i, Row ? i : n); }
	constexpr decltype(auto) get(Size i) const { return m.get(Row ? n : i, Row ? i : n); }
	constexpr decltype(auto) at(Size i) { return m.at(Row ? n : i, Row ? i : n); }
	constexpr decltype(auto) set(Size i, ConstReference val) {
		return m.set(Row ? n : i, Row ? i : n, val);
	}
};


template<typename M, bool Row, bool staticSized = M::staticSized>
struct MatProxyVec;

template<typename M, bool Row>
struct MatProxyVec<M, Row, true> : public MatProxyBase<M, Row> {
	using typename MatProxyBase<M, Row>::Size;
	using typename MatProxyBase<M, Row>::MatVec;

	static constexpr auto staticSized = true;
	static constexpr auto size() { return Row ? M::rows() : M::cols(); }

	template<Size D> static auto create() { return MatVec::template create<D>(); }
};

template<typename M, bool Row>
struct MatProxyVec<M, Row, false> : public MatProxyBase<M, Row> {
	using typename MatProxyBase<M, Row>::Size;
	using typename MatProxyBase<M, Row>::MatVec;

	static constexpr auto staticSized = false;
	typename M::Size size() const { return Row ? this->m.rows() : this->m.cols(); }

	auto create(Size size) { return MatVec::create(size); }
};

template<typename M> using MatRowProxy = MatProxyVec<M, true>;
template<typename M> using MatColProxy = MatProxyVec<M, false>;

} // namespace detail

/// \brief Prints the given matrix with numerical values to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types. Can be used to implement such an operator (as
/// [nytl::Mat]() does).
/// \param valueWidth The number of characters per matrix value. Can be 0 to not care for it.
/// \param rowSpacing The spacing in the beginning of each row.
/// \param breakAfter Whether to insert a newline after printing the matrix.
/// \requires There must be an implementation of operator<<(std::ostream&, M::Value).
template<typename M>
std::ostream& print(std::ostream& ostream, const M& mat, unsigned int valueWidth = 6,
	const char* rowSpacing = "    ", bool breakAfter = true)
{
	auto& os = templatize<M>(ostream);
	auto org = os.precision();
	os << "{" << "\n";

	// returns the number of digits needed to present the integer part of i
	const auto numberOfDigits = [](double i) {
		return (i < 10 && i >= 0) ? 1 : (i > -10 && i < 0) ? 2 :
			(i > 0) ? std::log10(i) + 1 : std::log10(-i) + 2;
	};

	for(auto r = 0u; r < mat.rows(); ++r) {
		os << rowSpacing << "(";

		for(auto c = 0u; c < mat.cols(); c++) {
			if(valueWidth) os.width(valueWidth);
			if(valueWidth) os.precision(valueWidth - numberOfDigits(mat.get(r, c)) - 1);

			os << mat.get(r, c);
			if(c != mat.cols() - 1)
				os << ", ";
		}

		os << ")" << "\n";
	}

	os << "}";
	if(breakAfter) os << "\n";
	if(valueWidth) os.precision(org);
	return os;
}

/// \brief Returns the row with index n of the given matrix.
/// For example: `nytl::mat::row(mat44, 0);` returns the first (index 0) row of a matrix.
/// \returns A M::RowVec holding the elements from the nth row.
template<typename M>
constexpr auto row(const M& mat, typename M::Size n)
{
	return detail::MatRowProxy<const M>{mat, n};
}

/// \brief Returns the column with index n of the given matrix.
/// For example: `nytl::mat::row(mat44, 1);` returns the second (index 1) column of a matrix.
/// \returns A M::ColVec holding the elements from the nth column.
template<typename M>
constexpr auto col(const M& mat, typename M::Size n)
{
	return detail::MatColProxy<const M>{mat, n};
}

/// \brief Sets the nth row of the given mutable matrix.
/// For example: `nytl::mat::row(mat44, 0, vec4);` sets the first row of a matrix.
/// \requires Type 'R' shall be a container that can be accessed using operator[] and
/// that holds at least as much values as mat has columns which can be converted to M::Value.
template<typename M, typename R>
constexpr void row(M& mat, typename M::Size n, const R& row)
{
	for(auto i = 0u; i < mat.cols(); ++i)
		mat.set(n, i, row.get(i));
}

/// \brief Sets the column with index n to the given column.
/// For example: `nytl::mat::col(mat44, 2, vec4);` sets the 3rd column of a matrix.
/// \requires Type 'R' shall be a container that can be accessed using operator[] and
/// that holds at least as much values as mat has rows which can be converted to M::Value.
template<typename M, typename C>
constexpr void col(M& mat, typename M::Size n, const C& col)
{
	for(auto i = 0u; i < mat.rows(); ++i)
		mat.set(i, n, col.get(i));
}

/// \brief Swaps the row with index n with the row with index i.
/// For example: `nytl::mat::swapRow(mat44, 2, 3);` swaps the 3rd and 4th row
/// The given matrix must be mutable.
template<typename M>
constexpr void swapRow(M& mat, typename M::Size n, typename M::Size i)
{
	using std::swap;
	for(auto c = 0u; c < mat.cols(); ++c) {
		auto tmp = mat.get(n, c);
		mat.set(n, c, mat.get(i, c));
		mat.set(i, c, tmp);
	}
}

/// \brief Swaps the column with index n with the column with index i.
/// For example: `nytl::mat::swapCol(mat44, 2, 3);` swaps the 3rd and 4th column
/// The given matrix must be mutable.
template<typename M>
constexpr void swapCol(M& mat, typename M::Size n, typename M::Size i)
{
	using std::swap;
	for(auto r = 0u; r < mat.rows(); ++r) {
		auto tmp = mat.get(r, n);
		mat.set(r, n, mat.get(r, i));
		mat.set(r, i, tmp);
	}
}

/// \brief Copies the second matrix into the first one.
/// Both matrices should have the same size, otherwise calling this functions
/// results in undefined behavior.
/// For example: `nytl::mat::copy(mat44f, mat44d);` copies the double matrix into the float one.
/// The first given matrix must be mutable.
/// \requires std::is_convertible<N::Value, M::Value>
template<typename M, typename N>
constexpr void copy(M& a, const N& b)
{
	for(auto r = 0u; r < a.rows(); ++r)
		for(auto c = 0u; c < a.cols(); ++c)
			a.set(r, c, b.get(r, c));
}

/// \brief Sets all values of the given matrix to 0 of the underlying field.
/// The given matrix must be mutable.
template<typename M>
constexpr void zero(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat.set(r, c, 0.0);
}

/// \brief Sets all values of the given matrix to 1 of the underlying field.
/// The given matrix must be mutable.
template<typename M>
constexpr void one(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat.get(r, c) = 1.0;
}

/// \brief Returns the trace of a square matrix, i.e. the sum of its diagonal elements
/// Undefined behavior for empty or non-square matrices.
template<typename M>
constexpr auto trace(const M& mat)
{
	auto ret = mat.get(0, 0);
	for(auto n = 1u; n < mat.rows(); ++n)
		ret += mat.get(n, n);
	return ret;
}

/// \brief Multiplies all elements in the diagonal of the given non-empty square matrix.
/// Undefined behavior for empty or non-square matrices.
template<typename M>
constexpr auto multiplyDiagonal(const M& mat)
{
	auto ret = mat.get(0, 0);
	for(auto n = 1u; n < mat.rows(); ++n)
		ret *= mat.get(n, n);
	return ret;
}

/// \brief Sets the given matrix to the identity matrix.
/// Undefined behavior for non-square matrices.
/// The given matrix must be mutable.
template<typename M>
constexpr void identity(M& mat)
{
	zero(mat);
	for(auto n = 0u; n < mat.rows(); ++n)
		mat.set(n, n, 1.0);
}

/// \brief Transposes the given matrix.
/// \returns A rebound matrix of the same implementation with C rows and R rows.
template<typename M>
constexpr auto transpose(const M& mat)
{
	auto ret = detail::createTransposeMatrix(mat);

	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			ret.set(c, r, mat.get(r, c));

	return ret;
}

/// \brief Performs partial pivoting for the given matrix for given position.
/// Finds the largest value in the given column and swaps its row with the given row.
/// Complexity Lies within O(n^2).
/// The given matrix must be mutable.
/// \param row The row of the matrix entry to maximize.
/// \param column The column of the matrix entry to maximize.
/// \param after If this is true, only rows after the given one are considered for swapping.
/// \returns The new value at the given position.
template<typename M>
constexpr auto pivot(M& mat, typename M::Size row, typename M::Size column, bool after = false)
{
	auto maxRow = row;
	for(auto r = after ? row + 1 : 0; r < mat.rows(); ++r)
		if(std::abs(mat.get(r, column)) > std::abs(mat.get(maxRow, column)))
			maxRow = r;

	if(maxRow != row)
		swapRow(mat, row, maxRow);

	return mat.get(row, column);
}

/// \brief Brings the given matrix into the row echolon form (ref).
/// Implements the first step of the Gaussian elimination algorithm for a given matrix.
/// The given matrix does not have to fulfill any requirements.
/// Does directly modify the matrix. For a version that operates on a copy, see
/// rowEcholonCopy.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \notes This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in quiet rounding errors here).
template<typename M>
constexpr void rowEcholon(M& mat)
{
	// Start with the topleft element and go one right in each step.
	// We only go one down if we could eliminate the current column with the current row.
	for(auto r = 0u, c = 0u; r < mat.rows() && c < mat.cols(); ++c) {
		// maximize the current pivot. Only consider rows after the current one.
		// go to the next column (continue without increasing the row) if the pivot is zero
		if(pivot(mat, r, c, true) == 0.0) continue;

		// Divide all elements in this row by the first element since it should be 1
		// we already assured that the pivot cannot be zero, so we can divide by it
		auto factor = mat.get(r, c);
		for(auto i = c; i < mat.cols(); ++i) mat.set(r, i, mat.get(r, i) / factor);

		// Now add a multiple of the current row to all other rows, so that this column
		// will be set to 0 everywhere except the current line
		for(auto i = r + 1; i < mat.rows(); ++i) {
			auto fac = mat.get(i, c);
			for(auto j = c; j < mat.cols(); ++j) {
				mat.set(i, j, mat.get(i, j) - fac * mat.get(r, j));
			}
		}

		++r; // go to the next row
	}
}

/// \brief Same as [nytl::mat::rowEcholon](), but operates on a copy.
/// Assures that the given matrix is converted to a matrix will full precision.
template<typename M>
constexpr auto rowEcholonCopy(const M& mat)
{
	auto ret = detail::createMatrix<double>(mat);
	copy(ret, mat);
	rowEcholon(ret);
	return ret;
}

/// \brief Brings the given matrix into the reduced row echolon form (rref).
/// Implements the full Gaussian elimination for a given matrix.
/// The given matrix can be in any form.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// The given matrix must be mutable.
/// \notes This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in errors here).
template<typename M>
constexpr void reducedRowEcholon(M& mat)
{
	// first bring the matrix into row-echolon form
	rowEcholon(mat);

	// start with the bottom and eliminate all coefficients above this row that
	// are in the same c
	for(auto r = mat.rows(); r-- > 0; ) {

		// find the pivot
		auto c = 0u;
		while(c < mat.cols() && mat.get(r, c) == typename M::Value {}) ++c;

		// if the pivot is zero continue to the next (above) row
		if(!mat.get(r, c)) continue;

		// eliminate other coefficients in the current column above the current row
		for(auto p = 0u; p < r; ++p) {
			auto fac = mat.get(p, c) / mat.get(r, c);
			for(auto q = 0u; q < mat.cols(); ++q) {
				mat.set(p, q, mat.get(p, q) - fac * mat.get(r, q));
			}
		}
	}
}

/// \brief Same as [nytl::mat::reducedRowEcholon](), but operates on a copy.
/// Assures that the given matrix is converted to a matrix will full precision.
template<typename M>
constexpr auto reducedRowEcholonCopy(const M& mat)
{
	auto ret = detail::createMatrix<double>(mat);
	copy(ret, mat);
	reducedRowEcholon(ret);
	return ret;
}

/// \brief Computes a LU decomposition of a given square matrix.
/// \returns std::tuple with the lower (0) and upper (1) matrix of the decomposition, as
/// well as the used permutation matrix (2) and the sign of the permutation (3).
/// The sign is a value of the field the matrix is defined over. It's 1 if an even number
/// of permutation were performed, or -1 if an uneven number of permutations were performed.
/// The returned lower matrix will have only ones in its diagonal.
/// Works for every square matrix, even singular ones like the zero matrix.
/// The returned matrices always fulfill the equation: PA = LU, where P is the permutation
/// matrix, A the given matrix, L the lower and U the upper matrix.
/// Read more about lu decomposition at [https://en.wikipedia.org/wiki/LU_decomposition]().
/// The returned matrices have the full field precision type, since this operation divides values.
/// This function cannot fail in any way.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
template<typename M>
constexpr auto luDecomp(const M& mat)
{
	using RetMat = decltype(detail::createMatrix<double>(mat));

	std::tuple<RetMat, RetMat, RetMat, int> ret {};
	auto& lower = (std::get<0>(ret) = detail::createMatrix<double>(mat));
	auto& upper = (std::get<1>(ret) = detail::createMatrix<double>(mat));
	auto& perm = (std::get<2>(ret) = detail::createMatrix<double>(mat));
	auto& sign = (std::get<3>(ret) = 1.0);

	identity(perm);
	copy(upper, mat);

	for(auto n = 0u; n < mat.cols(); ++n) {

		// since we divide by upper[n][n] later on we should try to make it non-zero by
		// swapping the current row with another row. If we do so, we have to pretend we
		// swapped the matrix in the beginning and therefore also change the lower matrix and
		// remember the swap in the permutation matrix
		if(upper.get(n, n) == 0) {
			for(auto r = n + 1; r < mat.rows(); ++r) {
				if(upper.get(r, n) != 0) {
					swapRow(perm, r, n);
					swapRow(upper, r, n);
					swapRow(lower, r, n);
					sign *= -1;
					break;
				}
			}

			// If all coefficients in the column are zero (e.g. a zero matrix), its ok since
			// we don't have any more coefficients to eliminate.
			if(upper.get(n, n) == 0) {
				lower.set(n, n, 1);
				continue;
			}
		}

		lower.set(n, n, 1);

		// erase all coefficients in the nth column below the nth row.
		// pivoting already assured that mat[n][n] is not zero
		auto rown = row(upper, n);
		for(auto i = n + 1; i < mat.rows(); ++i) {
			auto fac = static_cast<double>(upper.get(i, n)) / upper.get(n, n);
			auto rowi = row(upper, i);
			auto rowin = rowi - fac * rown;
			row(upper, i, rowin);
			lower.get(i, n) = fac;
		}
	}

	return ret;
}

namespace nocheck {

/// \brief Same as [nytl::mat::luEvaluate]() but does not perform any matrix checks.
template<typename M, typename V>
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	auto d = vec::detail::createVector<double>(b);
	auto x = vec::detail::createVector<double>(b);

	// forward substitution
	for(auto i = 0u; i < d.size(); ++i) {
		d.set(i, b.get(i));
		for(auto j = 0u; j < i; ++j)
			d.set(i, std::fma(-l.get(i, j), d[j], d[i]));

		d.set(i, d[i] / l.get(i, i));
	}

	// back substitution
	for(auto i = x.size(); i-- > 0; ) {
		x.set(i, d[i]);
		for(auto j = i + 1; j < x.size(); ++j)
			x.set(i, std::fma(-u.get(i, j), x[j], x[i]));

		x.set(i, x[i] / u.get(i, i));
	}

	return x;
}

} // namespace nocheck

/// \brief Returns the vector x so that LUx = b.
/// Can be used to more efficiently solve multiple linear equitation systems for the
/// same matrix by first decomposing it and then use this function instead of the default
/// Gaussian elimination implementation.
/// The given matrix must be a square matrix.
/// \notes If the lu composition was done with a permutation matrix (PA = LU), the given
/// vector must be premultiplied with the permutations inverse (tranpose) to get the vector
/// that solves Ax = b. If PA = LU and Ax = b, so LUx = P * b
/// \notes Does not check if the given equitation is solvable, i.e. results in undefined behavior
/// if it is not. The caller should check or assure this somehow. Could be done by
/// checking whether the given lower or upper matrix is singular, i.e. whether one of its
/// diagonal elements is zero.
/// The returned vector has a full field precision type, since this operation divides values.
/// Complexity Lies within O(n^2) where n is the number of rows/cols of the given matrix.
/// \requires Type 'V' shall be a vector that has as many elements as l and u have rows/columns.
template<typename M, typename V>
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	if(l.rows() != l.cols() || u.rows() != u.cols() || l.rows() != u.rows())
		throw std::invalid_argument("nytl::mat::luEvaluate: invalid lu matrices");

	for(auto n = 0u; n < l.rows(); ++n)
		if(l.get(n, n) == 0.0 || u.get(n, n) == 0.0)
			throw std::invalid_argument("nytl::mat::luEvaluate: singular lower or upper matrix");

	return nocheck::luEvaluate(l, u, b);
}

/// \brief Returns the determinant of the given square matrix.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
template<typename M>
constexpr auto determinant(const M& mat)
{
	auto [l, u, p, s] = luDecomp(mat);
	nytl::unused(l, p);
	return s * static_cast<typename M::Value>(multiplyDiagonal(u));
}

/// \brief Returns the determinant for the lu decomposition of a matrix.
/// \param l The lower matrix of the lu decomposition.
/// \param lu The upper matrix of the lu decomposition.
/// \param sign The sign of the permutation matrix used on the original matrix.
template<typename M>
constexpr auto determinant(const M& l, const M& u, int sign = 1)
{
	return sign * multiplyDiagonal(l) * multiplyDiagonal(u);
}

/// \brief Returns whether the given matrix can be inverted.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \returns false for non-square matrices.
template<typename M>
constexpr bool invertible(const M& mat)
{
	if(mat.rows() != mat.cols()) return false;
	return (determinant(mat) != 0);
}

namespace nocheck {

/// \brief Returns the inverse of a matrix A with PA = LU.
/// \notes Does not perform any sanity checks of the given matrix.
/// The matrix must be square-sized and invertible.
template<typename M>
constexpr auto inverse(const M& l, const M& u, const M& p)
{
	auto ret = detail::createMatrix<double>(l);
	for(auto i = 0u; i < ret.cols(); ++i)
		col(ret, i, nocheck::luEvaluate(l, u, col(p, i)));

	return ret;
}

/// \brief Returns the inverse of the Matrix A with A = LU.
/// \notes Does not perform any sanity checks of the given matrix.
/// The given matrices must be square-sized and invertible.
template<typename M>
constexpr auto inverse(const M& l, const M& u)
{
	auto p = detail::createMatrix<typename M::Value>(l);
	identity(p);
	return inverse(l, u, p);
}

/// \brief Same as [nytl::mat::inverse(const M& mat)]() but does not check for errors.
/// \notes Does not perform any sanity checks of the given matrix.
/// The matrix must be square-sized and invertible.
template<typename M>
constexpr auto inverse(const M& mat)
{
	auto [l, u, p, s] = luDecomp(mat);
	nytl::unused(s);
	return inverse(l, u, p);
}

} // namespace nocheck

/// \brief Returns the inverse of the matrix A with A = LU.
/// \throws std::invalid_argument For a non-square or singular matrix
template<typename M>
constexpr auto inverse(const M& l, const M& u)
{
	if(l.rows() != l.cols() || u.rows() != u.cols() || l.rows() != u.rows())
		throw std::invalid_argument("nytl::mat::inverse: non-square matrix");

	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == 0.0 || l[n][n] == 0.0)
			throw std::invalid_argument("nytl::mat::inverse: singular matrix");

	return nocheck::inverse(l, u);
}

/// \brief Returns the inverse of the matrix A with PA = LU.
/// \throws std::invalid_argument For a non-square or singular matrix
template<typename M>
constexpr auto inverse(const M& l, const M& u, const M& p)
{
	if(l.rows() != l.cols() || u.rows() != u.cols() || l.rows() != u.rows())
		throw std::invalid_argument("nytl::mat::inverse: non-square matrix");

	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == 0.0 || l[n][n] == 0.0)
			throw std::invalid_argument("nytl::mat::inverse: singular matrix");

	return nocheck::inverse(l, u, p);
}

/// \brief Returns the inverse of the given square matrix.
/// \throws std::invalid_argument for non-square or singular matrices.
/// Either catch the exception or check if the invertible matrix using
/// [nytl::mat::invertible](). One can also use [nytl::mat::invert(const M&)]()
/// to check if a matrix is invertible and calculate the inverse if so. This will
/// be way more efficient then first checking and then calculating it.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
template<typename M>
constexpr auto inverse(const M& mat)
{
	if(mat.rows() != mat.cols())
		throw std::invalid_argument("nytl::mat::inverse: non-square matrix");

	auto [l, u, p, s] = luDecomp(mat);
	nytl::unused(s);

	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == 0.0)
			throw std::invalid_argument("nytl::mat::inverse: singular matrix");

	return nocheck::inverse(l, u, p);
}

/// \brief Checks if the given matrix is invertible and inverts it if so.
/// \returns Whether the given matrix could be inverted, i.e. if the matrix
/// is an invertible square matrix.
/// If the matrix could not be inverted, false is returned and the matrix is left unchanged.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// The given matrix must be mutable.
template<typename M>
constexpr bool invert(M& mat)
{
	if(mat.rows() != mat.cols()) return false;

	auto [l, u, p, s] = luDecomp(mat); //TODO C++17

	// check for singular matrix
	for(auto n = 0u; n < l.rows(); ++n)
		if(u.get(n, n) == 0.0)
			return false;

	mat = nocheck::inverse(l, u, p);
	return true;
}

/// \brief Returns whether the given Matrix is symmetric.
/// Uses the == operator over M::Value to check for equality.
template<typename M>
constexpr bool symmetric(const M& mat)
{
	for(auto r = 1u; r < mat.rows(); ++r)
		for(auto c = 0u; c < r; ++c)
			if(mat.get(r, c) != mat.get(c, r))
				return false;
	return true;
}

} // namespace nytl

#endif // header guard
