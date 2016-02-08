/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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

namespace detail
{

//returns the plane (e.g. 0,1 for xy or 2,3 for zw) for a given id in a given dimension
Vec2<std::size_t> indexPlane(std::size_t dim, std::size_t idx)
{
	auto ret = Vec2<std::size_t> {};

	while(idx >= dim - 1)
	{
		ret[0]++;
		idx -= (dim - 1);
		dim--;
	}

	ret[1] = idx + 1;

	return ret;
}

}

//basic ops
///\relates Mat Transform
template<std::size_t D, typename P>
void scale(SquareMat<D, P>& mat, const Vec<D - 1, P>& scal)
{
	for(std::size_t i(0); i < D - 1; ++i)
		mat[i][i] *= scal[i];
}

///\relates Mat Transform
template<std::size_t D, typename P>
void translate(SquareMat<D, P>& mat, const Vec<D - 1, P>& trans)
{
	for(std::size_t i(0); i < D - 1; ++i)
		mat[D - 1][i] += trans[i];
}


///\relates Mat Transform
template<std::size_t D, typename P>
void rotate(SquareMat<D + 1, P>& mat, const Vec<rotationPlanes(D), P>& planes, P angle)
{
	rotate(angle * planes);
}

///\relates Mat Transform
template<std::size_t D, typename P>
void rotate(SquareMat<D + 1, P>& mat, const Vec<rotationPlanes(D), P>& planeRot)
{
	auto rotMat = identityMat<D>();
	for(std::size_t i(0); i < rotationPlanes(D); ++i)
	{
		auto planeMat = identityMat<D>();
		auto idx = detail::indexPlane(D, i);

		auto c = std::cos(planeRot[i]);
		auto s = std::sin(planeRot[i]);

		planeMat[idx[0]][idx[0]] = c;
		planeMat[idx[0]][idx[1]] = -s;
		planeMat[idx[1]][idx[0]] = s;
		planeMat[idx[1]][idx[1]] = c;

		rotMat *= planeMat;
	}	

	mat *= rotMat;
}

//copy helper
///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> scaleCopy(SquareMat<D, P> mat, const Vec<D, P>& scal)
{
	scale(mat, scal);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> translateCopy(SquareMat<D, P> mat, const Vec<D, P>& trans)
{
	translate(mat, trans);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> rotateCopy(SquareMat<D, P> mat, const Vec<D, P>& axes, const P& angle)
{
	rotate(mat, axes, angle);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> rotateCopy(SquareMat<D, P> mat, const Vec<D, P>& rot)
{
	rotate(mat, rot);
	return mat;
}

#ifdef DOXYGEN
}
#endif