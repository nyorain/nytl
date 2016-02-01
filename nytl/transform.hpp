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

///\file
///\brief Provides a transform class for translation, rotation and scaling of several primitives.

#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>
#include <nytl/rect.hpp>
#include <nytl/scalar.hpp>

#include <cmath>

namespace nytl
{

///\relates Transform
///Returns the possible axis of rotation for a given dimension.
constexpr std::size_t rotationAxis(std::size_t dimension) { return fac(dimension) / 2; }

///\ingroup math
///Utility base class for objects holding a transform state.
template<std::size_t D, typename P = float> class Transformable; //only for 2d, todo: 3d

///\ingroup math
///Transform state able to hold rotation,scale and translation and express them in a Matrix.
///
///\tparam D The dimension of transforMation.
///\tparam P the precision of the transforMations, defaulted to float.
///
///At the moment, there exist only valid specializations for 2D and 3D transform objects.
///
///The Transform class effectively just holds a SquareMat<D + 1, P> object in which it stores
///the applied transforMations. Therefore it is not possible to retrieve the scaling/rotation/
///translation values sperately from the Matrix.
///Since it does therefore apply all transforMations immediatly to the Matrix, the order in which
///the transforMations are applied may change the outcome.
///
///\code
///auto transform = nytl::transform2 {};
///
/////First translates and then rotates the transform Matrix.
////The rotation will be around the origin, so in respect of the previous translation.
///transform.translate({100, 100});
///transform.rotate(45);
///
/////First rotates and then translated the transform Matrix. 
///transform.rotate(45);
///transform.translate({100, 100});
///\endcode
///
///Alternatively to the transform object, one can use the raw nytl::transform, nytl::scale or
///nytl::rotate functions when operating diRectly on a Matrix, or use the nytl::Transformable
///convinence base class when developing own transformable classes.
template<std::size_t D, typename P = float> class Transform;

using Transformable2 = Transformable<2>;
using Transformable3 = Transformable<3>;

using Transform2 = Transform<2>;
using Transform3 = Transform<3>;

template<std::size_t D, typename P>
class Transform
{
public:
    static constexpr std::size_t dim = D;

	using Precision = P;
	using VecType = Vec<dim, Precision>;
	using MatType = SquareMat<dim + 1, Precision>;
	using RotType = typename VecScalar<rotationAxis(dim), Precision>::type;
	using RectType = Rect<dim, Precision>;

protected:
	MatType mat_ {};

public:
    Transform() noexcept : mat_(identityMat<dim + 1, P>()) {} 
    ~Transform() noexcept = default;

    void rotate(const RotType& rotation);
    void translate(const VecType& pos);
    void scale(const VecType& pscale);

	void reset() { identity(mat_); };

    MatType& mat() { return mat_; }
    const MatType& mat() const { return mat_; }
};


template<std::size_t D, typename P>
class Transformable
{
public:
    static constexpr std::size_t dim = D;
    using Precision = P;

    using TransformType = Transform<dim, Precision>;
	using VecType = typename TransformType::VecType;
	using MatType = typename TransformType::MatType;
	using RotType = typename TransformType::RotType;
	using RectType = typename TransformType::RectType;

protected:
    TransformType transform_;

public:
    Transformable() = default;
    ~Transformable() = default;

    void rotate(const RotType& rotation){ transform_.rotate(rotation); }
    void translate(const VecType& translation){ transform_.translate(translation); }
    void scale(const VecType& scaling){ transform_.scale(scaling); }

	void reset() { transform_.reset(); };

    MatType& transformMat() { return transform_.mat(); }
    const MatType& transformMat() const { return transform_.mat(); }

	TransformType& transform() { return transform_; }
	const TransformType& transform() const { return transform_; }
};

#include <nytl/bits/transform.inl>

}
