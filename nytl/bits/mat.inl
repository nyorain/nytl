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

//members
template<std::size_t R, std::size_t C, typename P, typename Cond>
template<std::size_t OR, std::size_t OC, typename OP>
mat<R, C, P, Cond>::operator mat<OR, OC, OP>() const
{
	mat<OR, OC, OP> ret({});
	detail::initMatData<min(OR, R) * min(OC, C)>::call(ret.data_, asTuple());
	return ret;
}

///\relates mat
template<std::size_t R, std::size_t C, typename P>
void swapRow(mat<R, C, P>& m, std::size_t a, std::size_t b)
{
	for(std::size_t i(0); i < C; ++i)
	{
		std::swap(m[a][i], m[b][i]);
	}
}

///\relates mat
template<std::size_t R, std::size_t C, typename P>
void swapCol(mat<R, C, P>& m, std::size_t a, std::size_t b)
{
	for(std::size_t i(0); i < R; ++i)
	{
		std::swap(m[i][a], m[i][b]);
	}
}

///\relates mat
///Returns the identityMatrix for the given dimension and precision.
template<size_t D, typename P = float>
squareMat<D, P> identityMat()
{
	squareMat<D, P> ret(0);
	for(size_t i(0); i < D; i++) ret[i][i] = 1;
	return ret;
}

//XXX: correct implementation.
//full pivot?
///\relates mat
///Returns the sign of the used pivot matrix.
template<std::size_t D, typename P>
int pivot(mat<D, D, P>& m)
{
	int ret = 1;

	for(std::size_t c(0); c < D; ++c)
	{
		std::size_t maxR = c;
		for(std::size_t r(c); r < D; ++r)
		{
			if(std::abs(m[r][c]) > std::abs(m[maxR][c]))
			maxR = r;

		}

		if(maxR != c)
		{
			swapRow(m, c, maxR);
			ret *= -1;
		}
	}

	return ret;
}

///\relates mat
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
template<std::size_t D, typename P>
P diagonalMult(const mat<D, D, P>& m)
{
	P ret = 1;
	for(std::size_t i(0); i < D; ++i)
		ret *= m[i][i];

	return ret;
}

///\relates mat
template<std::size_t D, typename P>
double det(const mat<D, D, P>& m)
{
	auto cpy = m;
	auto psign = pivot(cpy);
	auto luMats = luDecomposition(cpy);

	return diagonalMult(luMats[0]) * diagonalMult(luMats[1]) * psign;
}


//todo: algorithms not entirely correct, detect 2 fail cases and return them as error code
///\relates mat
template<size_t R, size_t C, typename P>
bool refMat(mat<R, C, P>& ma)
{
    for(size_t k = 0; k < std::min(R, C); ++k)
    {
        size_t iMax = 0;
        P iMaxValue {};

        for(size_t r = k; r < R; ++r)
        {
            if(std::abs(ma[r][k]) > iMaxValue)
            {
                iMaxValue = std::abs(ma[r][k]);
                iMax = r;
            }
        }

        if(ma[iMax][k] == 0) //singular matrix, throw here but first document it.
            return 0;

        std::swap(ma[k], ma[iMax]);

        for(size_t r = k + 1; r < R; ++r)
        {
            for(size_t c = k + 1; c < C; ++c)
            {
                ma[r][c] = ma[r][c] - ma[k][c] * (ma[r][k] / ma[k][k]);
            }

            ma[r][k] = 0;
        }
    }

    return 1;
}

///\relates mat
template<size_t R, size_t C, typename P>
bool rrefMat(mat<R, C, P>& ma)
{
    if(!refMat(ma))
        return 0;

    for(int k = R - 1; k >= 0; --k)
    {
        size_t leadingIndex = 0;
        for(; leadingIndex < C; ++leadingIndex)
            if(ma[k][leadingIndex] != 0)
                break;

        if(leadingIndex == C)
        {
            //std::cout << "empty row\n";
            return 0;
        }

        P fac = ma[k][leadingIndex];
        ma[k] /= fac;

        for(size_t r = 0; r < (size_t)k; ++r)
        {
            ma[r] -= ma[r][leadingIndex] * ma[k];
        }
    }

    return 1;
}

//TODO XXX:
//some function for analyzing the ref/rref result
//no solution, infinite solutions, exactly onesolution?

//operators
namespace detail
{


constexpr const unsigned int cDWidth = 6;
unsigned int getNumberOfDigits(double i)
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
				<< std::setprecision(cDWidth - getNumberOfDigits(obj[i][o]) - 1) 
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

