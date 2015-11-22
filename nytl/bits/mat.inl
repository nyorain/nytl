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


//identityMat
//can probably be done as c++11 constexpr with tmp
template<size_t dim, typename prec = float>
NYTL_CPP14_CONSTEXPR squareMat<dim, prec> identityMat()
{
	squareMat<dim, prec> ret{};
	for(size_t i(0); i < dim; i++) ret[i][i] = prec(1);
	return ret;
}

//mat utility
//todo: more standard functions
//todo: algorithms not entirely correct, detect 2 fail cases and return them as error code
template<size_t rows, size_t cols, typename prec>
NYTL_CPP14_CONSTEXPR bool refMat(mat<rows, cols, prec>& ma)
{
    for(size_t k = 0; k < std::min(rows, cols); ++k)
    {
        size_t iMax = 0;
        prec iMaxValue {};

        for(size_t r = k; r < rows; ++r)
        {
            if(std::abs(ma[r][k]) > iMaxValue)
            {
                iMaxValue = std::abs(ma[r][k]);
                iMax = r;
            }
        }

        if(ma[iMax][k] == 0) //singular matrix
            return 0;

        std::swap(ma[k], ma[iMax]);

        for(size_t r = k + 1; r < rows; ++r)
        {
            for(size_t c = k + 1; c < cols; ++c)
            {
                ma[r][c] = ma[r][c] - ma[k][c] * (ma[r][k] / ma[k][k]);
            }

            ma[r][k] = 0;
        }
    }

    return 1;
}

template<size_t rows, size_t cols, typename prec>
NYTL_CPP14_CONSTEXPR bool rrefMat(mat<rows, cols, prec>& ma)
{
    if(!mat_ref(ma))
        return 0;

    for(int k = rows - 1; k >= 0; --k)
    {
        size_t leadingIndex = 0;
        for(; leadingIndex < cols; ++leadingIndex)
            if(ma[k][leadingIndex] != 0)
                break;

        if(leadingIndex == cols)
        {
            //std::cout << "empty row\n";
            return 0;
        }

        prec fac = ma[k][leadingIndex];
        ma[k] /= fac;

        for(size_t r = 0; r < (size_t)k; ++r)
        {
            ma[r] -= ma[r][leadingIndex] * ma[k];
        }
    }

    return 1;
}

//operators
//ostream//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr const unsigned int cDWidth = 6;
constexpr unsigned int getNumberOfDigits(double i)
{
    return ((i < 10 && i > 0) || i == 0) ? 1 :
    (i > -10 && i < 0) ? 2 :
    (i > 0) ? (unsigned int) std::log10((double) i) + 1 : (unsigned int) std::log((double) -i) + 2;
}

template<size_t rows, size_t cols, class prec>
std::ostream& operator<<(std::ostream& os, const mat<rows, cols, prec>& obj)
{
    auto org = os.precision();
    os << "{" << "\n";

    for(unsigned int i(0); i < rows; i++)
    {
        os << "  " << "(";
        for(unsigned int o(0); o < cols; o++)
        {
            os << std::setw(cDWidth) << std::setprecision(cDWidth - getNumberOfDigits(obj[i][o]) - 1) << obj[i][o];
            if(o != cols - 1)
                os << ", ";
        }

        os << ")" << "\n";
    }

    os << "}";
    os.precision(org);

    return os;
}

//todo: more efficiency with wrapper classes for operations
//+////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t rows, size_t cols, typename prec>
constexpr mat<rows, cols, prec> operator+(mat<rows, cols, prec> ma, const mat<rows, cols, prec>& mb)
{
    return std::move(ma += mb);
}


//-////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t rows, size_t cols, typename prec> mat<rows, cols, prec>
constexpr operator-(mat<rows, cols, prec> ma, const mat<rows, cols, prec>& mb)
{
    return std::move(ma -= mb);
}


//*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//mat and value
template<size_t rows, size_t cols, typename prec>
constexpr mat<rows, cols, prec> operator*(mat<rows, cols, prec> ma, const prec& other)
{
    return std::move(ma *= other);
}

template<size_t rows, size_t cols, typename prec>
constexpr mat<rows, cols, prec> operator*(const prec& other, mat<rows, cols, prec> ma)
{
    return std::move(ma *= other);
}

//mat and mat
template<size_t rowsA, size_t colsA, size_t colsB, typename prec>
NYTL_CPP14_CONSTEXPR mat<rowsA, colsB, prec> operator*(const mat<rowsA, colsA, prec>& ma, const mat<colsA, colsB, prec>& mb)
{
    mat<rowsA, colsB, prec> ret {};

    for(size_t r(0); r < rowsA; ++r)
        for(size_t c(0); c < colsB; ++c)
            ret[r][c] = weight(ma.row(r) * mb.col(c));

    return ret;
}

//mat and vector
template<size_t rows, size_t cols, typename prec>
NYTL_CPP14_CONSTEXPR vec<rows, prec> operator*(const mat<rows, cols, prec>& ma, const vec<cols, prec>& v)
{
    vec<rows, prec> ret {};
    ret.fill(prec());

    for(size_t i(0); i < rows; i++)
        ret[i] = weight(ma.row(i) * v);

    return ret;
}

template<size_t rows, size_t cols, typename prec>
vec<rows, prec> operator*(const vec<cols, prec>& v, const mat<rows, cols, prec>& ma)
{
    return (ma * v);
}

