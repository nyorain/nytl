/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#ifdef DOXYGEN
namespace nytl{
#endif

//members
template<std::size_t R, std::size_t C, typename P>
template<std::size_t OR, std::size_t OC, typename OP>
mat<R, C, P>::operator mat<OR, OC, OP>() const
{
	mat<OR, OC, OP> ret({});
	detail::initMatData<min(OR, R) * min(OC, C)>::call(ret.data_, asTuple());
	return ret;
}

template<std::size_t R, std::size_t C, typename P>
template<typename TD>
typename std::enable_if<mat<R, C, P>::is_squared, TD>::type mat<R, C, P>::invert()
{
	//TODO	
}


//exceptions
///\relates mat
///\brief Exception class deriving std::invalid_argument.
///\detail Thrown by operational matrix functions that do not work for singular matrices
///but receive a singular matrix as argument.
class invalid_matrix : public std::invalid_argument
{
public:
	invalid_matrix()
		: std::invalid_argument("Invalid matrix argument") {}

	invalid_matrix(const std::string& func)
		: std::invalid_argument("Invalid matrix argument in function " + func) {}
};


///\relates mat
///Returns the identityMatrix for the given dimension and precision.
template<std::size_t D, typename P = float>
squareMat<D, P> identityMat()
{
	squareMat<D, P> ret(0);
	for(size_t i(0); i < D; i++) ret[i][i] = 1;
	return ret;
}

///\relates mat
///\return The inverse of the given square matrix
template<std::size_t D, typename P> 
squareMat<D, P> inverse(const squareMat<D, P>& m)
{
	auto cpy = m;
	return m.invert();
}

//XXX: correct implementation?
//full pivot?

///\relates mat
///\brief Rearranges the matrix rows.
///\return The sign of the used pivot matrix.
template<std::size_t R, std::size_t C, typename P>
int pivot(mat<R, C, P>& m)
{
	int ret = 1;

	for(std::size_t c(0); c < C; ++c)
	{
		std::size_t maxR = c;
		for(std::size_t r(c); r < R; ++r)
		{
			if(std::abs(m[r][c]) > std::abs(m[maxR][c]))
			maxR = r;
		}

		if(maxR != c)
		{
			m.swapRow(c, maxR);
			ret *= -1;
		}
	}

	return ret;
}

///\relates mat
///\brief Computes a luDecomposition of a non-singular matrix.
///\return 2 mats, The lower (l, first mat) and the upper one(u, second mat).
///\warning May throw a nytl::invalid_matrix exception if the given matrix is
///not correctly pivotized.
template<std::size_t D, typename P>
vec2<mat<D, D, double>> luDecomposition(const mat<D, D, P>& m)
{
	vec2<mat<D, D, P>> lu{};
	lu[0] = identityMat<D, P>();
	lu[1].fill(0);

	for(std::size_t r(0); r < D; ++r)
	{
		for(std::size_t c(0); c < D; ++c)
		{
			P vsum = 0;

			//u
			if(c >= r)
			{
				for(std::size_t k(0); k < r; ++k)
					vsum += lu[1][k][c] * lu[0][r][k];

				lu[1][r][c] = m[r][c] - vsum;
			}

			//l
			else
			{
				if(lu[1][c][c] == 0)
				{
					throw invalid_matrix("nytl::luDecomposition, needs pivoting");
				}

				vsum = 0;

				for(std::size_t k(0); k < c; ++k)
					vsum += lu[1][k][c] * lu[0][r][k];

				lu[0][r][c] = (m[r][c] - vsum) / lu[1][c][c];
			}
		}
	}

	return lu;	
}

///\relates mat
///\brief Composutes the product of all diagonal elements of a square-mat.
template<std::size_t D, typename P>
P diagonalMult(const mat<D, D, P>& m)
{
	P ret = 1;
	for(std::size_t i(0); i < D; ++i)
		ret *= m[i][i];

	return ret;
}

///\relates mat
///\brief Computes the determinant of a given non-singular matrix.
///\todo May throw (since lu-algorithm is used). Determinant always computable.
template<std::size_t D, typename P>
double det(const mat<D, D, P>& m)
{
	auto cpy = m;
	auto psign = pivot(cpy);
	auto luMats = luDecomposition(cpy);

	return diagonalMult(luMats[0]) * diagonalMult(luMats[1]) * psign;
}


///\relates mat
///\brief Brings a given mat in the row-echolon-form (ref).
///\details The given mat does not have to be pivotized.
template<std::size_t R, std::size_t C, typename P>
void refMat(mat<R, C, P>& m)
{
	std::size_t c = 0;
    for(std::size_t r = 0; r < R; ++r, ++c)
    {
		for(; c < C; ++c)
		{
			//basically pivotize
			std::size_t maxR = r;
			for(std::size_t r2 = r + 1; r2 < R; ++r2)
			{
				if(std::abs(m[r2][c]) > std::abs(m[maxR][c]))
					maxR = r2;
			}

			if(m[maxR][c] != 0)
			{
				if(r != maxR) m.swapRow(r, maxR);
				break;
			}
			else if(c == C - 1)
			{
				return;
			}
		}

		m[r] /= m[r][c];

		for(std::size_t r2 = r + 1; r2 < R; ++r2)
		{
			if(m[r2][c] != 0)
			{
				m[r2] -= (m[r2][c] / m[r][c]) * m[r];
			}
		}
	}
}

///\relates mat
///\brief Returns the row-echolon-form (ref) of a given mat.
template<size_t R, size_t C, typename P>
mat<R, C, P> refMatCopy(mat<R, C, P> m)
{
	refMat(m);
	return m;
}

/*
//TODO: Some kind of solution set class for possible matrix solutions?
///\relates mat
///Analzyes a matrix in row echelon form
///\return
///Returns 0 if the corresponding linear equotation system is not solvable.
///Returns 1 if it is unabiguously solveavle by exactly one solution.
///Returns 2 if it has infinity solutions.
template<size_t R, size_t C, typename P>
unsigned int analyzeRefMat(const mat<R, C, P>& m)
{
	//TODO
	return 0;
}
*/

///\relates mat
///\brief Brings a given matrix in the reduced-row-echolon-form (rref). 
///\details The mat will first be brought into the row-echolon-form, so it does not have
///to fulfill any requirements.
template<size_t R, size_t C, typename P>
void rrefMat(mat<R, C, P>& m)
{
    refMat(m);

    for(int r = R - 1; r >= 0; --r)
    {
		std::size_t c = 0;
        for(; c < C; ++c)
		{
			if(m[r][c] != 0) break;
		}

		if(m[r][c] == 0)
		{
			continue;
		}

        m[r] /= m[r][c];

        for(std::size_t r2 = 0; r2 < r; ++r2)
        {
            m[r2] -= m[r2][c] * m[r];
        }
    }
}

///\relates mat
///\brief Computes the reduced-row-echolon-form (rref) of a given mat.
template<size_t R, size_t C, typename P>
mat<R, C, P> rrefMatCopy(mat<R, C, P> m)
{
	rrefMat(m);
	return m;
}

//TODO XXX:
//some function for analyzing the ref/rref result
//no solution, infinite solutions, exactly onesolution?

//operators
namespace detail
{


constexpr const unsigned int cDWidth = 6;
inline unsigned int getNumberOfDigits(double i)
{
    return ((i < 10 && i > 0) || i == 0) ? 1 :
    (i > -10 && i < 0) ? 2 :
    (i > 0) ? (unsigned int) std::log10((double) i) + 1 : (unsigned int) std::log((double) -i) + 2;
}

}

///\relates mat
template<size_t R, size_t C, class P>
std::ostream& operator<<(std::ostream& os, const mat<R, C, P>& obj)
{
    auto org = os.precision();
    os << "{" << "\n";

    for(unsigned int i(0); i < R; i++)
    {
        os << "  " << "(";
        for(unsigned int o(0); o < C; o++)
        {
			using namespace detail;
            os	<< std::setw(cDWidth) 
				<< std::setprecision(cDWidth - getNumberOfDigits(obj[i][o]) + 4) 
				<< obj[i][o];

            if(o != C - 1)
                os << ", ";
        }

        os << ")" << "\n";
    }

    os << "}";
    os.precision(org);

    return os;
}

//todo: more efficiency with wrapper classes for operations
//+
///\relates mat
template<size_t R, size_t C, typename P>
mat<R, C, P> operator+(mat<R, C, P> ma, const mat<R, C, P>& mb)
{
    return std::move(ma += mb);
}


//-
///\relates mat
template<size_t R, size_t C, typename P> mat<R, C, P>
operator-(mat<R, C, P> ma, const mat<R, C, P>& mb)
{
    return std::move(ma -= mb);
}


//*
//mat and value
///\relates mat
template<size_t R, size_t C, typename P>
mat<R, C, P> operator*(mat<R, C, P> ma, const P& other)
{
    return std::move(ma *= other);
}

///\relates mat
template<size_t R, size_t C, typename P>
mat<R, C, P> operator*(const P& other, mat<R, C, P> ma)
{
    return std::move(ma *= other);
}

//mat and mat
///\relates mat
template <size_t RA, size_t CA, size_t CB, typename P> mat<RA, CB, P> 
operator*(const mat<RA, CA, P>& ma, const mat<CA, CB, P>& mb)
{
    mat<RA, CB, P> ret {};

    for(size_t r(0); r < RA; ++r)
        for(size_t c(0); c < CB; ++c)
            ret[r][c] = weight(ma.row(r) * mb.col(c));

    return ret;
}

//mat and vector
///\relates mat
template<size_t R, size_t C, typename P>
vec<R, P> operator*(const mat<R, C, P>& ma, const vec<C, P>& v)
{
    vec<R, P> ret {};
    ret.fill(P());

    for(size_t i(0); i < R; i++)
        ret[i] = weight(ma.row(i) * v);

    return ret;
}

///\relates mat
template<size_t R, size_t C, typename P>
vec<R, P> operator*(const vec<C, P>& v, const mat<R, C, P>& ma)
{
    return (ma * v);
}

#ifdef DOXYGEN
}
#endif
