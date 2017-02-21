// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various matrix related operations.

#pragma once

#ifndef NYTL_INCLUDE_MAT_OPS
#define NYTL_INCLUDE_MAT_OPS

#include <nytl/tmpUtil.hpp> // nytl::templatize

#include <utility> // std::swap
#include <stdexcept> // std::invalid_argument
#include <tuple> // std::tuple
#include <iosfwd> // std::ostream
#include <cmath> // std::fma

namespace nytl::mat {

/// \brief Prints the given matrix with numerical values to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types. Can be used to implement such an operator (as
/// [nytl::Mat]() does).
/// \param valueWidth The number of characters per matrix value. Can be 0 to not care for it.
/// \param rowSpacing The spacing in the beginning of each row.
/// \param breakAfter Whether to insert a newline after printing the matrix.
/// \requires Type 'V' shall be a Vector
/// \requires There must be an implementation of operator<<(std::ostream&, V::Value).
/// \module matOps
template<typename M>
std::ostream& print(std::ostream& ostream, const M& mat, unsigned int valueWidth = 6,
	const char* rowSpacing = "    ", bool breakAfter = true)
{
	auto& os = templatize<M>(ostream);
	auto org = os.precision();
	os << "{" << "\n";

	// returns the number of digitis needed to present the integer part of i
	const auto numberOfDigits = [](double i) {
		return (i < 10 && i >= 0) ? 1 : (i > -10 && i < 0) ? 2 :
			(i > 0) ? std::log10(i) + 1 : std::log10(-i) + 2;
	};

	for(auto r = 0u; r < mat.rows(); ++r) {
		os << rowSpacing << "(";

		for(auto c = 0u; c < mat.cols(); c++) {
			if(valueWidth) os.width(valueWidth);
			if(valueWidth) os.precision(valueWidth - numberOfDigits(mat[r][c]) - 1);

			os << mat[r][c];
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
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto row(const M& mat, typename M::Size n)
{
	typename M::RowVec ret {};
	for(auto i = 0u; i < mat.cols(); ++i)
		ret[i] = mat[n][i];

	return ret;
}

/// \brief Returns the column with index n of the given matrix.
/// For example: `nytl::mat::row(mat44, 1);` returns the second (index 1) column of a matrix.
/// \returns A M::ColVec holding the elements from the nth column.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto col(const M& mat, typename M::Size n)
{
	typename M::ColVec ret {};
	for(auto i = 0u; i < mat.rows(); ++i)
		ret[i] = mat[i][n];

	return ret;
}

/// \brief Sets the nth row of the given Matrix.
/// For example: `nytl::mat::row(mat44, 0, vec4);` sets the first row of a matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \requires Type 'R' shall be a Container that can be accessed using operator[] and
/// that holds at least as much values as mat has columns which can be converted to M::Value.
/// \module matOps
template<typename M, typename R>
constexpr void row(M& mat, typename M::Size n, const R& row)
{
	for(auto i = 0u; i < mat.cols(); ++i)
		mat[n][i] = row[i];
}

/// \brief Sets the column with index n to the given column.
/// For example: `nytl::mat::col(mat44, 2, vec4);` sets the 3rd column of a matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \requires Type 'R' shall be a Container that can be accessed using operator[] and
/// that holds at least as much values as mat has rows which can be converted to M::Value.
/// \module matOps
template<typename M, typename C>
constexpr void col(M& mat, typename M::Size n, const C& col)
{
	for(auto i = 0u; i < mat.rows(); ++i)
		mat[i][n] = col[i];
}

/// \brief Swaps the row with index n with the row with index i.
/// For example: `nytl::mat::swapRow(mat44, 2, 3);` swaps the 3rd and 4th row
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr void swapRow(M& mat, typename M::Size n, typename M::Size i)
{
	using std::swap;
	for(auto c = 0u; c < mat.cols(); ++c)
		swap(mat[n][c], mat[i][c]);
}

/// \brief Swaps the column with index n with the column with index i.
/// For example: `nytl::mat::swapCol(mat44, 2, 3);` swaps the 3rd and 4th column
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr void swapCol(M& mat, typename M::Size n, typename M::Size i)
{
	using std::swap;
	for(auto r = 0u; r < mat.rows(); ++r)
		swap(mat[r][n], mat[r][i]);
}

/// \brief Copies the second matrix into the first one.
/// Both matrices should have the same size, otherwise calling this functions
/// results in undefined behaviour.
/// For example: `nytl::mat::copy(mat44f, mat44d);` copies the double matrix into the float one.
/// \requires Types 'M', 'N' shall be matrix types. 'M' shall be mutable.
/// \requires The value type of 'N' must be convertible to the value type of 'M'.
/// \module matOps
template<typename M, typename N>
constexpr void copy(M& a, const N& b)
{
	for(auto r = 0u; r < a.rows(); ++r)
		for(auto c = 0u; c < a.cols(); ++c)
			a[r][c] = b[r][c];
}

/// \brief Sets all values of the given matrix to 0 of the underlaying field.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr void zero(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat[r][c] = 0.0;
}

/// \brief Sets all values of the given matrix to 1 of the underlaying field.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr void one(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat[r][c] = 1.0;
}

/// \brief Returns the trace of a square matrix, i.e. the sum of its diagonal elements
/// Undefined behaviour for empty or non-sqaure matrices.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto trace(const M& mat)
{
	auto ret = mat[0][0];
	for(auto n = 1u; n < mat.rows(); ++n)
		ret += mat[n][n];
	return ret;
}

/// \brief Multiplies all elements in the diagonal of the given non-empty square matrix.
/// Undefined behaviour for empty or non-square matrices.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto multiplyDiagonal(const M& mat)
{
	auto ret = mat[0][0];
	for(auto n = 1u; n < mat.rows(); ++n)
		ret *= mat[n][n];
	return ret;
}

/// \brief Sets the given matrix to the identity matrix.
/// Undefined behaviour for non-square matrices.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr void identity(M& mat)
{
	zero(mat);
	for(auto n = 0u; n < mat.rows(); ++n)
		mat[n][n] = 1.0;
}

/// \brief Transposes the given matrix.
/// \returns A rebound matrix of the same implementation with C rows and R rows.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto transpose(const M& mat)
{
	auto ret = typename M::template Rebind<M::colDim, M::rowDim, typename M::Value> {};

	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			ret[c][r] = mat[r][c];

	return ret;
}

/// \brief Performs partical pivoting for the given matrix for given position.
/// Finds the largest value in the given column and swaps its row with the given row.
/// Complexity Lies within O(n^2).
/// \param row The row of the matrix entry to maximize.
/// \param column The column of the matrix entry to maximize.
/// \param after If this is true, only rows after the given one are considered for swapping.
/// \returns The new value at the given position.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr auto pivot(M& mat, typename M::Size row, typename M::Size column, bool after = false)
{
	auto maxRow = row;
	for(auto r = after ? row + 1 : 0; r < mat.rows(); ++r)
		if(std::abs(mat[r][column]) > std::abs(mat[maxRow][column]))
			maxRow = r;

	if(maxRow != row)
		swapRow(mat, row, maxRow);

	return mat[row][column];
}

/// \brief Brings the given matrix into the row echolon form (ref).
/// Implements the first step of the gaussian elimination algorithm for a given matrix.
/// The given matrix does not have to fulfill any requirements.
/// Does directly modify the matrix. For a version that operates on a copy, see
/// rowEcholonFormCopy.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \notes This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in quiet rounding errors here).
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
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
		auto factor = mat[r][c];
		for(auto i = c; i < mat.cols(); ++i) mat[r][i] /= factor;

		// Now add a multiple of the current row to all other rows, so that this column
		// will be set to 0 everywhere except the current line
		for(auto i = r + 1; i < mat.rows(); ++i) {
			auto fac = mat[i][c];
			for(auto j = c; j < mat.cols(); ++j) {
				mat[i][j] -= fac * mat[r][j];
			}
		}

		++r; // go to the next row
	}
}

/// \brief Same as [nytl::mat::rowEcholon](), but operates on a copy.
/// Assures that the given matrix is converted to a matrix will full precision.
/// \module matOps
template<typename M>
constexpr auto rowEcholonCopy(const M& mat)
{
	using RetMat = typename M::template Rebind<M::rowDim, M::colDim, double>;

	auto ret = RetMat::create(mat.rows(), mat.cols());
	copy(ret, mat);
	rowEcholon(ret);
	return ret;
}

/// \brief Brings the given matrix into the reduced row echolon form (ref).
/// Implements the full gaussian elimination for a given matrix.
/// The given matrix can be in any form.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \notes This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in errors here).
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr void reducedRowEcholon(M& mat)
{
	// first bring the matrix into row-echolon form
	rowEcholon(mat);

	// start with the bottom and eliminate all coefficients above this row that
	// are in the same c
	for(auto r = mat.rows(); r-- > 0; ) {

		//find the pivot
		auto c = 0u;
		while(c < mat.cols() && mat[r][c] == typename M::Value {}) ++c;

		// if the pivot is zero continue to the next (above) row
		if(!mat[r][c]) continue;

		// eliminate other coefficients in the current column above the current row
		for(auto p = 0u; p < r; ++p) {
			auto fac = mat[p][c] / mat[r][c];
			for(auto q = 0u; q < mat.cols(); ++q) {
				mat[p][q] -= fac * mat[r][q];
			}
		}
	}
}

/// \brief Same as [nytl::mat::reducedRowEcholon](), but operates on a copy.
/// Assures that the given matrix is converted to a matrix will full precision.
/// \module matOps
template<typename M>
constexpr auto reducedRowEcholonCopy(const M& mat)
{
	using RetMat = typename M::template Rebind<M::rowDim, M::colDim, double>;

	auto ret = RetMat::create(mat.rows(), mat.cols());
	copy(ret, mat);
	reducedRowEcholon(ret);
	return ret;
}

/// \brief Computes a LU decomposition of a given matrix.
/// \returns std::tuple with the lower (0) and upper (1) matrix of the decomposition, as
/// well as the used permutation matrix (2) and the sign of the permutation (3).
/// The sign is a value of the field the matrix is defined over. It's 1 if an even number
/// of permutation were performed, or -1 if an uneven number of permutations were performed.
/// The returned lower matrix will have only ones in its diagonal.
/// Works for every square matrix, even singular ones like the zero matrix.
/// The returned matrices always fullfill the equtation: PA = LU, where P is the permutation
/// matrix, A the given matrix, L the lower and U the upper matrix.
/// Read more about lu decomposition at [https://en.wikipedia.org/wiki/LU_decomposition]().
/// The returned matrices have the full field precision type, since this operation divides values.
/// This function cannot fail in any way.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a square Matrix.
/// \module matOps
template<typename M>
constexpr auto luDecomp(const M& mat)
{
	using RetMat = typename M::template Rebind<M::rowDim, M::colDim, double>;

	std::tuple<RetMat, RetMat, RetMat, int> ret {};
	auto& lower = (std::get<0>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& upper = (std::get<1>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& perm = (std::get<2>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& sign = (std::get<3>(ret) = 1.0);

	identity(perm);
	copy(upper, mat);

	for(auto n = 0u; n < mat.cols(); ++n) {

		// since we divide by upper[n][n] later on we should try to make it non-zero by
		// swapping the current row with another row. If we do so, we have to pretend we
		// swapped the matrix in the beginning and therefore also change the lower matrix and
		// remember the swap in the permutation matrix
		if(upper[n][n] == 0.0) {
			for(auto r = n + 1; r < mat.rows(); ++r) {
				if(upper[r][n] != 0.0) {
					swapRow(perm, r, n);
					swapRow(upper, r, n);
					swapRow(lower, r, n);
					sign *= -1;
					break;
				}
			}

			// If all coefficients in the column are zero (e.g. a zero matrix), its ok since
			// we don't have any more coefficients to eliminate.
			if(upper[n][n] == 0.0) {
				lower[n][n] = 1.0;
				continue;
			}
		}

		lower[n][n] = 1.0;

		// erase all coefficients in the nth column below the nth row.
		// pivoting already assured that mat[n][n] is not zero
		auto rown = row(upper, n);
		for(auto i = n + 1; i < mat.rows(); ++i) {
			auto fac = static_cast<double>(upper[i][n]) / upper[n][n];
			auto rowi = row(upper, i);
			auto rowin = rowi - fac * rown;
			row(upper, i, rowin);
			lower[i][n] = fac;
		}
	}

	return ret;
}

namespace nocheck {

/// \brief Same as [nytl::mat::luEvaluate]() but does not perform any matrix checks.
/// \module matOps
template<typename M, typename V>
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	using RetVec = typename V::template Rebind<V::dim, double>;

	auto d = RetVec::create(b.size());
	auto x = RetVec::create(b.size());

	// forward substitution
	for(auto i = 0u; i < d.size(); ++i) {
		d[i] = b[i];
		for(auto j = 0u; j < i; ++j)
			d[i] = std::fma(-l[i][j], d[j], d[i]);

		d[i] /= l[i][i];
	}

	// back substitution
	for(auto i = x.size(); i-- > 0; ) {
		x[i] = d[i];
		for(auto j = i + 1; j < x.size(); ++j)
			x[i] = std::fma(-u[i][j], x[j], x[i]);

		x[i] /= u[i][i];
	}

	return x;
}

} // namespace nocheck

/// \brief Returns the vector x so that LUx = b.
/// Can be used to more efficiently solve multiple linear equotation systems for the
/// same matrix by first decomposing it and then use this function instead of the default
/// gaussian elimination implementation.
/// \notes If the lu composition was done with a permutation matrix (PA = LU), the given
/// vector must be premultiplied with the permutations inverse (tranpose) to get the vector
/// that solves Ax = b. If PA = LU and Ax = b, so LUx = P * b
/// \notes Does not check if the given equotation is solvable, i.e. results in undefined behaviour
/// if it is not. The caller should check or assure this somehow. Could be done by
/// checking whether the given lower or upper matrix is singular, i.e. whether one of its
/// diagonal elements is zero.
/// The returned vector has a full field precision type, since this operation divides values.
/// Complexity Lies within O(n^2) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a square Matrix.
/// \requires Type 'V' shall be a vector that has as many elements as l and u have rows/columns.
/// \module matOps
template<typename M, typename V>
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	if(l.rows() != l.cols() || u.rows() != u.cols() || l.rows() != u.rows())
		throw std::invalid_argument("nytl::mat::luEvaluate: invalid lu matrices");

	for(auto n = 0u; n < l.rows(); ++n)
		if(l[n][n] == 0.0 || u[n][n] == 0.0)
			throw std::invalid_argument("nytl::mat::luEvaluate: singular lower or upper matrix");

	return nocheck::luEvaluate(l, u, b);
}

/// \brief Returns the determinant of the given square matrix.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a Matrix.
/// \requires The given matrix shall be a square matrix.
/// \module matOps
template<typename M>
constexpr auto determinant(const M& mat)
{
	// auto [l, u, p, s] = luDecomp(mat); // TODO: C++17

	auto lups = luDecomp(mat);
	auto& u = std::get<1>(lups);
	auto& s = std::get<3>(lups);

	return s * static_cast<typename M::Value>(multiplyDiagonal(u));
}

/// \brief Returns the determinant for the lu decomposition of a matrix.
/// \param l The lower matrix of the lu decomposition.
/// \param lu The upper matrix of the lu decomposition.
/// \param sign The sign of the permutation matrix used on the original matrix.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr auto determinant(const M& l, const M& u, int sign = 1)
{
	return sign * multiplyDiagonal(l) * multiplyDiagonal(u);
}

/// \brief Returns whether the given matrix can be inversed.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \returns false for non-square matrices.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr bool invertible(const M& mat)
{
	if(mat.rows() != mat.cols()) return false;
	return (determinant(mat) != 0);
}

namespace nocheck {

/// \brief Returns the inverse of the matirx A with PA = LU.
/// \notes Does not perform any sanity checks of the given matrix.
/// \requires Type 'M' shall be an invertible square Matrix.
/// \module matOps
template<typename M>
constexpr auto inverse(const M& l, const M& u, const M& p)
{
	using RetMat = typename M::template Rebind<M::rowDim, M::colDim, double>;
	auto ret = RetMat::create(l.rows(), l.cols());

	for(auto i = 0u; i < ret.cols(); ++i)
		col(ret, i, nocheck::luEvaluate(l, u, col(p, i)));

	return ret;
}

/// \brief Returns the inverse of the matirx A with A = LU.
/// \notes Does not perform any sanity checks of the given matrix.
/// \requires Type 'M' shall be an invertible square Matrix.
/// \module matOps
template<typename M>
constexpr auto inverse(const M& l, const M& u)
{
	auto p = M::create(l.rows(), l.cols());
	identity(p);
	return inverse(l, u, p);
}

/// \brief Same as [nytl::mat::inverse(const M& mat)]() but does not check for errors.
/// \notes Does not perform any sanity checks of the given matrix.
/// \requires Type 'M' shall be an invertible square Matrix.
/// \module matOps
template<typename M>
constexpr auto inverse(const M& mat)
{
	// auto [l, u, p, s] = luDecomp(mat); //TODO C++17
	auto lups = luDecomp(mat);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);
	const auto& p = std::get<2>(lups);

	return inverse(l, u, p);
}

} // namespace nocheck

/// \brief Returns the inverse of the matrix A with A = LU.
/// \throws std::invalid_argument For a non-square or singular matrix
/// \module matOps
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
/// \module matOps
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
/// \requires Type 'M' shall be a Matrix type.
/// \module matOps
template<typename M>
constexpr auto inverse(const M& mat)
{
	if(mat.rows() != mat.cols())
		throw std::invalid_argument("nytl::mat::inverse: non-square matrix");

	// auto [l, u, p, s] = luDecomp(mat); //TODO C++17
	auto lups = luDecomp(mat);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);
	const auto& p = std::get<2>(lups);

	return inverse(l, u, p);

	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == 0.0)
			throw std::invalid_argument("nytl::mat::inverse: singular matrix");

	return nocheck::inverse(l, u, p);
}

/// \brief Checks if the given matrix is invertibe and inverts it if so.
/// \returns Whether the given matrix could be inverted, i.e. if the matrix
/// is an invertible square matrix.
/// If the matrix could not be inverted, false is returned and the matrix is left unchanged.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module matOps
template<typename M>
constexpr bool invert(M& mat)
{
	if(mat.rows() != mat.cols()) return false;

	// auto [l, u, p, s] = luDecomp(mat); //TODO C++17
	auto lups = luDecomp(mat);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);
	const auto& p = std::get<2>(lups);

	// check for singular matrix
	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == 0.0)
			return false;

	mat = nocheck::inverse(l, u, p);
	return true;
}

/// \brief Returns whether the given Matrix is symmetrix
/// Uses the == operator over M::Value to check for equality.
/// \requires Type 'M' shall be a Matrix.
/// \module matOps
template<typename M>
constexpr bool symmetric(const M& mat)
{
	for(auto r = 1u; r < mat.rows(); ++r)
		for(auto c = 0u; c < r; ++c)
			if(mat[r][c] != mat[c][r])
				return false;
	return true;
}

} // namespace nytl

#endif // header guard
