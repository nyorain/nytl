// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various matrix related operations.

#pragma once

#ifndef NYTL_INCLUDE_MAT_OPS
#define NYTL_INCLUDE_MAT_OPS

#include <nytl/field.hpp> // nytl::FieldTraits
#include <nytl/tmpUtil.hpp> // nytl::templatize

#include <utility> // std::swap
#include <stdexcept> // std::invalid_argument
#include <tuple> // std::tuple
#include <iosfwd> // std::ostream

#include <iostream> // TODO

namespace nytl::mat {

/// struct Matrix {
/// public:
/// 	using Size = ...; // usually std::size_t. Must be convertible from/to int.
/// 	using Value = ...; // value type, mathematical field
/// 	using RowType = ...; // Vector type able to hold one row of this matrix
/// 	using ColType = ...; // Vector type able to hold one column of this matrix
///
/// 	// Rebinds the Matrix implementation
/// 	template<typename T, Size MaxR, Size MaxC> using Rebind = ...;
///
/// 	// dimensions of the matrix. Might be symbolic constants
/// 	static constexpr Size maxRows = ..;
/// 	static constexpr Size maxCols = ..;
///
/// 	// creates a matrix for the given rows and cols.
/// 	static Matrix create(Size rows, Size cols);
///
/// public:
/// 	// matrix[r][c] must return a reference to the value of matrix in row r and column c.
///  	auto operator[](Size); // must return some kind of referencing vector.
///  	const auto operator[](Size) const; // must return some kind of referencing vector.
///
/// 	Size rows() const; // the number of rows
/// 	Size cols() const; // the number of columns
/// };
///
/// // NOTE: As specified in the Vector concept, vectors are interpreted column vectors.
/// // The matrix vector multiplication operators must be implemented this way.
/// // Invalid operations (such as multiply 3x2 matrix with 4x4 matrix) should not be implemented
/// // or throw an exception.
///
/// auto operator*(Value, Matrix);
/// auto operator*(Matrix, Matrix); // default matrix multiplication
/// auto operator*(Matrix, Vector); // multiplication of matrix with vector
/// auto operator*(Vector, Matrix); // multiplication of vector with 1-row matrix
/// auto operator+(Matrix, Matrix);
/// auto operator-(Matrix, Matrix);
/// bool operator==(Matrix, Matrix);
/// bool operator!=(Matrix, Matrix);

/// \brief Prints the given matrix with numerical values to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types.
/// \param valueWidth The number of characters per matrix value. Can be 0 to not care for it.
/// \param rowSpacing The spacing in the beginning of each row.
/// \param breakAfter Whether to insert a newline after printing the matrix.
/// \requires Type 'V' shall be a Vector
/// \requires There must be an implementation of operator<<(std::ostream&, V::Value).
/// \module vec
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

/// \brief Returns the nth row of the given matrix.
/// \returns A M::RowVec holding the elements from the nth row.
/// \requires Type 'M' shall be a Matrix.
/// \module mat
template<typename M>
constexpr auto row(const M& mat, typename M::Size n)
{
	typename M::RowVec ret {};
	for(auto i = 0u; i < mat.cols(); ++i)
		ret[i] = mat[n][i];

	return ret;
}

/// \brief Returns the nth column of the given matrix.
/// \returns A M::ColVec holding the elements from the nth column.
/// \requires Type 'M' shall be a Matrix.
/// \module mat
template<typename M>
constexpr auto col(const M& mat, typename M::Size n)
{
	typename M::ColVec ret {};
	for(auto i = 0u; i < mat.rows(); ++i)
		ret[i] = mat[i][n];

	return ret;
}

/// \brief Sets the nth row of the given Matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \requires Type 'R' shall be a Container that can be accessed using operator[] and
/// that holds at least as much values as mat has columns which can be converted to M::Value.
/// \module mat
template<typename M, typename R>
constexpr void row(M& mat, typename M::Size n, const R& row)
{
	for(auto i = 0u; i < mat.cols(); ++i)
		mat[n][i] = row[i];
}

/// \brief Sets the nth column of the given matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \requires Type 'R' shall be a Container that can be accessed using operator[] and
/// that holds at least as much values as mat has rows which can be converted to M::Value.
/// \module mat
template<typename M, typename C>
constexpr void col(M& mat, typename M::Size n, const C& col)
{
	for(auto i = 0u; i < mat.rows(); ++i)
		mat[i][n] = col[i];
}

/// \brief Swaps the nth with the ith row of the given matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module mat
template<typename M>
constexpr void swapRow(M& mat, typename M::Size n, typename M::Size i)
{
	using std::swap;
	for(auto c = 0u; c < mat.cols(); ++c)
		swap(mat[n][c], mat[i][c]);
}

/// \brief Swaps the nth with the ith column of the given matrix.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module mat
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
/// \requires Types 'M', 'N' shall be matrix types. 'M' shall be mutable.
/// \requires The value type of 'N' must be convertible to the value type of 'M'.
/// \module mat
template<typename M, typename N>
constexpr void copy(M& a, const N& b)
{
	for(auto r = 0u; r < a.rows(); ++r)
		for(auto c = 0u; c < a.cols(); ++c)
			a[r][c] = b[r][c];
}

/// \brief Sets all values of the given matrix to 0 of the underlaying field.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module mat
template<typename M>
constexpr void zero(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat[r][c] = FieldTraits<typename M::Value>::zero;
}

/// \brief Sets all values of the given matrix to 1 of the underlaying field.
/// \requires Type 'M' shall be a mutable Matrix.
/// \module mat
template<typename M>
constexpr void one(M& mat)
{
	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			mat[r][c] = FieldTraits<typename M::Value>::one;
}

/// \brief Returns the trace of a square matrix, i.e. the sum of its diagonal elements
/// Undefined behaviour for empty or non-sqaure matrices.
/// \requires Type 'M' shall be a Matrix.
/// \module mat
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
/// \module mat
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
/// \module mat
template<typename M>
constexpr void identity(M& mat)
{
	zero(mat);
	for(auto n = 0u; n < mat.rows(); ++n)
		mat[n][n] = FieldTraits<typename M::Value>::one;
}

/// \brief Transposes the given matrix.
/// \returns A rebound matrix of the same implementation with C rows and R rows.
/// \requires Type 'M' shall be a Matrix.
/// \module mat
template<typename M>
constexpr auto transpose(const M& mat)
{
	auto ret = typename M::template Rebind<typename M::Value, M::maxCols, M::maxRows> {};

	for(auto r = 0u; r < mat.rows(); ++r)
		for(auto c = 0u; c < mat.cols(); ++c)
			ret[c][r] = mat[r][c];

	return ret;
}

/// \brief Performs partical pivoting for the given matrix for given position.
/// Finds the largest value in the given column and swaps its row with the given row.
/// \param row The row of the matrix entry to maximize.
/// \param column The column of the matrix entry to maximize.
/// \param after If this is true, only rows after the given one are considered for swapping.
/// \returns The new value at the given position.
/// \requires Type 'M' shall be a mutable Matrix.
/// \complexity Lies within O(n^2).
/// \module mat
template<typename M>
constexpr auto pivot(M& mat, typename M::Size row, typename M::Size column, bool after = false)
{
	using Field = FieldTraits<typename M::Value>;

	auto maxRow = row;
	for(auto r = after ? row + 1 : 0; r < mat.rows(); ++r)
		if(Field::abs(mat[r][column]) > Field::abs(mat[maxRow][column]))
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
/// \requires Type 'M' shall be a Matrix.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \module mat
template<typename M>
constexpr void rowEcholon(M& mat)
{
	auto fieldZero = FieldTraits<typename M::Value>::zero;

	// Start with the topleft element and go one right in each step.
	// We only go one down if we could eliminate the current column with the current row.
	for(auto r = 0u, c = 0u; r < mat.rows() && c < mat.cols(); ++c) {
		// maximize the current pivot. Only consider rows after the current one.
		// go to the next column (continue without increasing the row) if the pivot is zero
		if(pivot(mat, r, c, true) == fieldZero) continue;

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

/// \brief Brings the given matrix into the reduced row echolon form (ref).
/// Implements the full gaussian elimination for a given matrix.
/// The given matrix can be in any form.
/// \requires Type 'M' shall be a Matrix.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \module mat
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
			auto fac = mat[p][c];
			for(auto q = 0u; q < mat.cols(); ++q) {
				mat[p][q] -= fac * mat[r][q];
			}
		}
	}
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
/// This function cannot fail in any way.
/// \requires Type 'M' shall be a square Matrix.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \moduloe mat
template<typename M>
constexpr auto luDecomp(const M& mat)
{
	using RetValue = typename FieldTraits<typename M::Value>::FullPrecision;
	using RetMat = typename M::template Rebind<RetValue, mat.maxRows, mat.maxCols>;

	auto fieldZero = FieldTraits<typename M::Value>::zero;
	auto fieldOne = FieldTraits<typename M::Value>::one;

	std::tuple<RetMat, RetMat, RetMat, int> ret {};
	auto& lower = (std::get<0>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& upper = (std::get<1>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& perm = (std::get<2>(ret) = RetMat::create(mat.rows(), mat.cols()));
	auto& sign = (std::get<3>(ret) = fieldOne);

	identity(perm);
	copy(upper, mat);

	// first pivot the matrix in a way that assures that the pivot we divide with is never 0
	// for(auto c = 0u; c < mat.cols(); ++c) {
	//
	// 	auto val = fieldZero;
	// 	for(auto i = 0u; i < c; ++i)
	// 		if(upper[i][i] != fieldZero)
	// 			val += (mat[c][i] / mat[i][i]) * mat[i][c];
	//
	// 	// try to swap a row so that mat[c][c] is not value
	// 	// if it would be it will be zero as pivot which is only acceptable if all
	// 	// values in the column are 0 (in which case we never swap the row and simply
	// 	// skip the column later on)
	// 	if(mat[c][c] != val) continue;
	// 	for(auto r = c + 1; r < mat.rows(); ++r) {
	// 		if(mat[r][c] != val) {
	// 			swapRow(upper, r, c);
	// 			swapRow(perm, r, c);
	// 			sign *= -1;
	// 			break;
	// 		}
	// 	}
	// }

	for(auto n = 0u; n < mat.cols(); ++n) {

		lower[n][n] = fieldOne;

		if(upper[n][n] == fieldZero) {
			for(auto r = n + 1; r < mat.rows(); ++r) {
				if(upper[r][n] != fieldZero) {
					swapRow(perm, r, n);
					swapRow(upper, r, n);
					swapRow(lower, r, n);
					sign *= -1;
					break;
				}
			}

			if(upper[n][n] == fieldZero) continue;
		}

		// erase all coefficients in the nth column below the nth row.
		// pivoting already assured that mat[n][n] is not zero
		auto rown = row(upper, n);
		for(auto i = n + 1; i < mat.rows(); ++i) {
			// std::cout << "    " << "[" << i << "]" << "[" << n << "] = " << upper[i][n] << "\n";
			// std::cout << "    " << "[" << n << "]" << "[" << n << "] = " << upper[n][n] << "\n";

			auto fac = static_cast<RetValue>(upper[i][n]) / upper[n][n];
			// std::cout << "    fac = " << fac << "\n";

			auto rowi = row(upper, i);
			auto rowin = rowi - fac * rown;
			row(upper, i, rowin);
			lower[i][n] = fac;

			// std::cout << "    " << i << ", " << fac << ", " << upper[i][n] << ", " << upper[n][n] << ": " << rowin << "\n";
			// std::cout << "\n";
		}

		// std::cout << n << ": " << rown << "\n";
		// std::cout << upper << "\n";
	}

	return ret;
}

/// \brief Returns the vector x so that LUx = b.
/// Can be used to more efficiently solve multiple linear equotation systems for the
/// same matrix by first decomposing it and then use this function instead of the default
/// gaussian elimination implementation.
/// Note that if the lu composition was done with a permutation matrix (PA = LU), the returned
/// vector must be multiplied with the permutation matrixs inverse (tranpose) to get the vector
/// that solves Ax = b.
/// \note Does not check if the given equotation is solvable, i.e. results in undefined behaviour
/// if it is not. The caller should check or assure this somehow. Could be done by
/// checking whether the given lower or upper matrix is singular, i.e. whether one of its
/// diagonal elements is zero.
/// \requires Type 'M' shall be a square Matrix.
/// \requires Type 'V' shall be a vector that has as many elements as l and u have rows/columns.
/// \complexity Lies within O(n^2) where n is the number of rows/cols of the given matrix.
/// \module mat
template<typename M, typename V>
constexpr auto luEvaluate(const M& l, const M& u, const V& b)
{
	// implemented with help of gamedev.net article https://goo.gl/CgPGvm
	using RetValue = typename FieldTraits<typename M::Value>::FullPrecision;
	using RetVec = typename V::template Rebind<V::dim, RetValue>;

	auto d = RetVec::create(b.size());
	auto x = RetVec::create(b.size());

	// forward substitution
	for(auto i = 0u; i < d.size(); ++i) {
		d[i] = b[i];
		for(auto j = 0u; j < i; ++j)
			d[i] -= l[i][j] * d[j];

		d[i] /= l[i][i];
	}

	// back substitution
	for(auto i = x.size(); i-- > 0; ) {
		x[i] = d[i];
		for(auto j = i + 1; j < x.size(); ++j)
			x[i] -= u[i][j] * x[j];

		x[i] /= u[i][i];
	}


	return x;
}

/// \brief Returns the determinant of the given square matrix.
/// \requires Type 'M' shall be a Matrix.
/// \requires The given matrix shall be a square matrix.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \module mat
template<typename M>
constexpr auto determinant(const M& mat)
{
	// auto [l, u, p, s] = luDecomp(mat); // TODO: C++17

	auto lups = luDecomp(mat);
	auto& u = std::get<1>(lups);
	auto& s = std::get<3>(lups);

	return s * multiplyDiagonal(u);
}

/// \brief Returns the determinant for the lu decomposition of a matrix.
/// \param l The lower matrix of the lu decomposition.
/// \param lu The upper matrix of the lu decomposition.
/// \param sign The sign of the permutation matrix used on the original matrix.
/// \required Type 'M' shall be a Matrix.
/// \module mat
template<typename M>
constexpr auto determinant(const M& l, const M& u, int sign = 1)
{
	return sign * multiplyDiagonal(l) * multiplyDiagonal(u);
}

/// \brief Returns whether the given matrix can be inversed.
/// Returns false for non-square matrices.
/// \requires Type 'M' shall be a Matrix.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \module mat
template<typename M>
constexpr bool invertible(const M& mat)
{
	if(mat.rows() != mat.cols()) return false;
	return (determinant(mat) != 0);
}

/// \brief Returns the inverse of the given square matrix.
/// \throws std::invalid_argument for non-square or singular matrices.
/// Either catch the exception or check if the invertible matrix using
/// [nytl::invertible](). One can also use [nytl::inverse<M, N>(const M&, N& inverse)]()
/// to check if a matrix is invertible and calculate the inverse if so. This will
/// be way more efficient then first checking and then calculating it.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a Matrix type.
/// \module mat
template<typename M>
constexpr auto inverse(const M& mat)
{
	if(mat.rows() != mat.cols()) throw std::invalid_argument("nytl::inverse: non-square matrix");

	using RetValue = typename FieldTraits<typename M::Value>::FullPrecision;
	using RetMat = typename M::template Rebind<RetValue, M::maxRows, M::maxCols>;
	auto ret = RetMat::create(mat.rows(), mat.cols());

	// auto [l, u, p, s] = luDecomp(mat); //TODO C++17

	auto lups = luDecomp(mat);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);

	std::cout << l << "\n" << u << "\n";

	// check for singular matrix
	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == FieldTraits<typename M::Value>::zero)
			throw std::invalid_argument("nytl::inverse: singular matrix");

	auto inversePerm = transpose(std::get<2>(lups));
	auto idmat = M::create(mat.rows(), mat.cols());
	identity(idmat);

	for(auto i = 0u; i < ret.cols(); ++i) {
		auto column = luEvaluate(l, u, row(idmat, i)); // row in this case same as col
		col(ret, i, inversePerm * column); // set the ith column
	}

	return ret;
}

/// \brief Checks if the given matrix is invertibe and inverts it if so
/// \returns Whether the given matrix could be inverted, i.e. if the matrix
/// is singular or non-square.
/// \param inverse The matrix in which the inverse will be stored. Will not be changed
/// if the matrix cannot be inverted.
/// \complexity Lies within O(n^3) where n is the number of rows/cols of the given matrix.
/// \requires Type 'M' shall be a mutable matrix.
/// \module mat
template<typename M>
constexpr bool inverse(const M& mat, M& inverse)
{
	if(mat.rows() != mat.cols()) return false;

	using ValueType = decltype(mat[0][0] / mat[0][0] - mat[0][0]);
	using RetType = typename M::template Rebind<ValueType, M::maxRows, M::maxCols>;
	auto ret = RetType::create(mat.rows(), mat.cols());

	// auto [l, u, p, s] = luDecomp(mat); //TODO C++17

	auto lups = luDecomp(mat);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);

	// check for singular matrix
	for(auto n = 0u; n < l.rows(); ++n)
		if(u[n][n] == FieldTraits<typename M::Value>::zero)
			return false;

	auto inversePerm = transpose(std::get<2>(lups));
	auto idmat = M::create(mat.rows(), mat.cols());
	identity(idmat);

	for(auto i = 0u; i < ret.cols(); ++i) {
		auto column = luEvaluate(l, u, row(idmat, i)); // row in this case same as col
		col(ret, i, inversePerm * column); // set the ith column
	}

	return ret;
}

// TODO:
//  - other lu algorithms, rework current one (with parameters), e.g. crout decomp
//  - throw if not solvable? divide by 0 (luEvaluate)?
// 		- general error handling concept

/// \module vec mat
/// \brief Returns whether the given vectors are linearly independent.
/// \tparam Args The given vectors. Must all be of the same type.
// template<typename... Args>
// constexpr auto linearlyIndependent(const Args&... args)
// {
// 	// Try to find a non-trivial combination of the nullvector from the given vectors
// }

}

#endif // header guard
