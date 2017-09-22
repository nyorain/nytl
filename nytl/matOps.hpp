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
#include <nytl/mat.hpp> // nytl::Mat
#include <nytl/vecOps.hpp> // nytl::dot

#include <utility> // std::swap
#include <stdexcept> // std::invalid_argument
#include <tuple> // std::tuple
#include <iosfwd> // std::ostream
#include <cmath> // std::fma

namespace nytl {

/// \brief Prints the given matrix with numerical values to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types. Can be used to implement such an operator (as
/// [nytl::Mat]() does).
/// \param valueWidth The number of characters per matrix value. Can be 0 to not care for it.
/// \param rowSpacing The spacing in the beginning of each row.
/// \param breakAfter Whether to insert a newline after printing the matrix.
/// \requires There must be an implementation of operator<<(std::ostream&, M::Value).
template<size_t R, size_t C, typename T>
std::ostream& print(std::ostream& ostream, const Mat<R, C, T>& mat, 
	unsigned int valueWidth = 6, const char* rowSpacing = "    ", bool breakAfter = true)
{
	auto& os = templatize<T>(ostream);
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

template<typename T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Mat<R, C, T>& a)
{
	return print(os, a);
}

/// \brief Returns the column with index n of the given matrix.
/// For example: `nytl::mat::row(mat44, 1);` returns the second (index 1) column of a matrix.
template<size_t R, size_t C, typename T>
constexpr auto col(const nytl::Mat<R, C, T>& mat, size_t n)
{
	Vec<R, T> ret;
	for(auto i = 0u; i < R; ++i)
		ret[i] = mat[i][n];
	return ret;
}

/// \brief Sets the column with index n to the given column.
/// For example: `nytl::mat::col(mat44, 2, vec4);` sets the 3rd column of a matrix.
template<size_t R, size_t C, typename T>
constexpr void col(nytl::Mat<R, C, T>& mat, size_t n, const nytl::Vec<R, T>& col)
{
	for(auto i = 0u; i < R; ++i)
		mat[i][n] = col[i];
}

/// \brief Swaps the row with index n with the row with index i.
/// For example: `nytl::mat::swapRow(mat44, 2, 3);` swaps the 3rd and 4th row
template<size_t R, size_t C, typename T>
constexpr void swapRow(nytl::Mat<R, C, T>& mat, size_t n, size_t i)
{
	using std::swap;
	swap(mat[n], mat[i]);
}

/// \brief Swaps the column with index n with the column with index i.
/// For example: `nytl::mat::swapCol(mat44, 2, 3);` swaps the 3rd and 4th column
template<size_t R, size_t C, typename T>
constexpr void swapCol(nytl::Mat<R, C, T>& mat, size_t n, size_t i)
{
	using std::swap;
	for(auto r = 0u; r < R; ++r)
		swap(mat[r][n], mat[r][i]);
}

/// \brief Sets all values of the given matrix to 1.
template<size_t R, size_t C, typename T>
constexpr void one(nytl::Mat<R, C, T>& mat)
{
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			mat[r][c] = T{1};
}

/// \brief Returns the trace of a square matrix, i.e. the sum of its diagonal elements
template<size_t D, typename T>
constexpr auto trace(const nytl::Mat<D, D, T>& mat)
{
	auto ret = mat[0][0];
	for(auto n = 1u; n < D; ++n)
		ret += mat[n][n];
	return ret;
}

/// \brief Multiplies all elements in the diagonal of the given non-empty square matrix.
template<size_t D, typename T>
constexpr auto multiplyDiagonal(const nytl::Mat<D, D, T>& mat)
{
	auto ret = mat[0][0];
	for(auto n = 1u; n < D; ++n)
		ret *= mat[n][n];
	return ret;
}

/// \brief Sets the given matrix to the identity matrix.
template<size_t D, typename T>
constexpr void identity(nytl::Mat<D, D, T>& mat)
{
	mat = {};
	for(auto n = 0u; n < D; ++n)
		mat[n][n] = T{1};
}

/// \brief Transposes the given matrix.
/// \returns A rebound matrix of the same implementation with C rows and R rows.
template<size_t R, size_t C, typename T>
constexpr auto transpose(const nytl::Mat<R, C, T>& mat)
{
	nytl::Mat<C, R, T> ret {};
	for(auto r = 0u; r < R; ++r)
		for(auto c = 0u; c < C; ++c)
			ret[c][r] = mat[r][c];

	return ret;
}

/// \brief Performs partial pivoting for the given matrix for given position.
/// Finds the largest value in the given column and swaps its row with the given row.
/// Complexity Lies within O(n^2).
/// \param row The row of the matrix entry to maximize.
/// \param column The column of the matrix entry to maximize.
/// \param onlyAfter If this is true, only rows after the given one are considered for swapping.
/// \returns The new value at the given position.
template<size_t R, size_t C, typename T>
constexpr auto pivot(nytl::Mat<R, C, T>& mat, size_t row, size_t column, bool onlyAfter = false)
{
	auto maxRow = row;
	for(auto r = onlyAfter ? row + 1 : 0; r < R; ++r)
		if(std::abs(mat[r][column]) > std::abs(mat[maxRow][column]))
			maxRow = r;

	if(maxRow != row)
		swapRow(mat, row, maxRow);

	return mat[row][column];
}

/// \brief Brings the given matrix into the row echolon form (ref).
/// Implements the first step of the Gaussian elimination algorithm for a given matrix.
/// The given matrix does not have to fulfill any requirements.
/// Does directly modify the matrix. For a version that operates on a copy, see
/// rowEcholonCopy.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \note This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in quiet rounding errors here).
template<size_t R, size_t C, typename T>
constexpr void rowEcholon(nytl::Mat<R, C, T>& mat)
{
	// Start with the topleft element and go one right in each step.
	// We only go one down if we could eliminate the current column with the current row.
	for(auto r = 0u, c = 0u; r < R && c < C; ++c) {
		// maximize the current pivot. Only consider rows after the current one.
		// go to the next column (continue without increasing the row) if the pivot is zero
		if(pivot(mat, r, c, true) == 0.0) {
			continue;
		}

		// Divide all elements in this row by the first element since it should be 1
		// we already assured that the pivot cannot be zero, so we can divide by it
		auto factor = mat[r][c];
		for(auto i = c; i < C; ++i) {
			mat[r][i] /= factor;
		}

		// Now add a multiple of the current row to all other rows, so that this column
		// will be set to 0 everywhere except the current line
		for(auto i = r + 1; i < R; ++i) {
			auto fac = mat[i][c];
			for(auto j = c; j < C; ++j) {
				mat[i][j] -= fac * mat[r][j];
			}
		}

		++r; // go to the next row
	}
}

/// \brief Same as [nytl::mat::rowEcholon](), but operates on a copy.
/// Gives the new matrix TN (= double) precision type to assure
/// there will be no rounding issues.
template<size_t R, size_t C, typename T, typename TN = double>
constexpr auto rowEcholonCopy(const nytl::Mat<R, C, T>& mat)
{
	auto ret = static_cast<nytl::Mat<R, C, TN>>(mat);
	rowEcholon(ret);
	return ret;
}

/// \brief Brings the given matrix into the reduced row echolon form (rref).
/// Implements the full Gaussian elimination for a given matrix.
/// The given matrix can be in any form.
/// Complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \note This operation divides by values from the matrix so it must have a type does
/// correctly implement division over the desired field (e.g. integer matrices will result
/// in errors here).
template<size_t R, size_t C, typename T>
constexpr void reducedRowEcholon(nytl::Mat<R, C, T>& mat)
{
	// first bring the matrix into row-echolon form
	rowEcholon(mat);

	// start with the bottom and eliminate all coefficients above this row that
	// are in the same c
	for(auto r = R; r-- > 0; ) {

		//find the pivot
		auto c = 0u;
		while(c < mat.cols() && mat[r][c] == T{0}) {
			++c;
		}

		// if the pivot is zero continue to the next (above) row
		if(!mat[r][c]) {
			continue;
		}

		// eliminate other coefficients in the current column above the current row
		for(auto p = 0u; p < r; ++p) {
			auto fac = mat[p][c] / mat[r][c];
			for(auto q = 0u; q < C; ++q) {
				mat[p][q] -= fac * mat[r][q];
			}
		}
	}
}

/// \brief Same as [nytl::mat::reducedRowEcholon](), but operates on a copy.
/// Gives the new matrix TN (= double) precision type to assure
/// there will be no rounding issues.
template<size_t R, size_t C, typename T, typename TN = double>
constexpr auto reducedRowEcholonCopy(const nytl::Mat<R, C, T>& mat)
{
	auto ret = static_cast<nytl::Mat<R, C, TN>>(mat);
	reducedRowEcholon(ret);
	return ret;
}

/// The return type of a LU decomposition.
template<size_t D, typename T>
struct LUDecomposition {
	nytl::Mat<D, D, T> lower;
	nytl::Mat<D, D, T> upper;
	nytl::Mat<D, D, bool> perm; // permutation
	unsigned int sign = 0;
};

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
template<size_t D, typename T>
constexpr LUDecomposition<D, double> luDecomp(const nytl::Mat<D, D, T>& mat)
{
	LUDecomposition<D, double> ret;
	identity(ret.perm);
	ret.upper = static_cast<decltype(ret.upper)>(mat);

	for(auto n = 0u; n < D; ++n) {

		// since we divide by upper[n][n] later on we should try to make it non-zero by
		// swapping the current row with another row. If we do so, we have to pretend we
		// swapped the matrix in the beginning and therefore also change the lower matrix and
		// remember the swap in the permutation matrix
		if(ret.upper[n][n] == 0.0) {
			for(auto r = n + 1; r < D; ++r) {
				if(ret.upper[r][n] != 0.0) {
					swapRow(ret.perm, r, n);
					swapRow(ret.upper, r, n);
					swapRow(ret.lower, r, n);
					ret.sign *= -1;
					break;
				}
			}

			// If all coefficients in the column are zero (e.g. a zero matrix), its ok since
			// we don't have any more coefficients to eliminate.
			if(ret.upper[n][n] == 0.0) {
				ret.lower[n][n] = 1.0;
				continue;
			}
		}

		ret.lower[n][n] = 1.0;

		// erase all coefficients in the nth column below the nth row.
		// pivoting already assured that mat[n][n] is not zero
		auto rown = row(ret.upper, n);
		for(auto i = n + 1; i < D; ++i) {
			auto fac = ret.upper[i][n] / ret.upper[n][n];
			auto rowi = row(ret.upper, i);
			auto rowin = rowi - fac * rown;
			row(ret.upper, i, rowin);
			ret.lower[i][n] = fac;
		}
	}

	return ret;
}

/*
/// \brief Returns the vector x so that LUx = b.
/// Can be used to more efficiently solve multiple linear equitation systems for the
/// same matrix by first decomposing it and then use this function instead of the default
/// Gaussian elimination implementation.
/// The given matrix must be a square matrix.
/// \notes If the lu composition was done with a permutation matrix (PA = LU), the given
/// vector must be premultiplied with the permutations inverse (tranpose) to get the vector
/// that solves Ax = b. If PA = LU and Ax = b, so LUx = P * b
/// \notes Does not check if the given equitation is solvable, i.e. results in undefined behavior
/// if it is not. The caller should check or assure this. Could be done by
/// checking whether the given lower or upper matrix is singular, i.e. whether one of its
/// diagonal elements is zero.
/// The returned vector has a full field precision type, since this operation divides values.
/// Complexity Lies within O(n^2) where n is the number of rows/cols of the given matrix.
template<size_t R, size_t D, 
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	// forward substitution
	for(auto i = 0u; i < d.size(); ++i) {
		d[i] = b[i];
		for(auto j = 0u; j < i; ++j)
			d[i] -= -l[i][j] * d[j];

		d[i] /= l[i][i];
	}

	// back substitution
	for(auto i = x.size(); i-- > 0; ) {
		x[i] = d[i];
		for(auto j = i + 1; j < x.size(); ++j)
			x[i] -= -u[i][j] * x[j];

		x[i] /= u[i][i];
	}

	return x;
}


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
		if(l[n][n] == 0.0 || u[n][n] == 0.0)
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
		if(u[n][n] == 0.0)
			return false;

	mat = nocheck::inverse(l, u, p);
	return true;
}
*/

/// \brief Returns whether the given quadratic matrix is symmetric.
template<size_t D, typename T>
constexpr bool symmetric(const nytl::Mat<D, D, T>& mat)
{
	for(auto r = 0u; r < D; ++r)
		for(auto c = r + 1; c < D; ++c)
			if(mat[r][c] != mat[c][r])
				return false;
	return true;
}

} // namespace nytl

#endif // header guard
