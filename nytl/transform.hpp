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
///Transform state able to hold rotation,scale and translation and express them in a matrix.
///
///\tparam D The dimension of transformation.
///\tparam P the precision of the transformations, defaulted to float.
///
///At the moment, there exist only valid specializations for 2D and 3D transform objects.
///
///The Transform class effectively just holds a squareMat<D + 1, P> object in which it stores
///the applied transformations. Therefore it is not possible to retrieve the scaling/rotation/
///translation values sperately from the matrix.
///Since it does therefore apply all transformations immediatly to the matrix, the order in which
///the transformations are applied may change the outcome.
///
///\code
///auto transform = nytl::transform2 {};
///
/////First translates and then rotates the transform matrix.
///transform.translate({100, 100});
///transform.rotate(45);
///
/////First rotates and then translated the transform matrix. 
/////The translation will take place in a rotated coordinate system.
///transform.rotate(45);
///transform.translate({100, 100});
///\endcode
///
///Alternatively to the transform object, one can use the raw nytl::transform, nytl::scale or
///nytl::rotate functions when operating directly on a matrix, or use the nytl::Transformable
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
    Transform() noexcept : mat_(identityMat<dim + 1, prec>()) {} 
    ~Transform() noexcept = default;

    void rotate(const RotType& rotation);
    void translate(const VecType& pos);
    void scale(const VecType& pscale);

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

	QuatType rotation_ {};
	VecType scaling_ {};
	VecType translation_ {};

public:
    Transformable() = default;
    ~Transformable() = default;

    template<size_t odim, typename oprec> 
		transformable(const transformable<odim, oprec>& other) : transform_(other.transform()) {}
    template<size_t odim, typename oprec> 
		transformable(const transform<odim, oprec>& trans) : transform_(trans) {}
    template<size_t odim, typename oprec> 
		transformable<dim, prec>& operator=(const transformable<odim, oprec>& other) 
		{ transform_ = other.transform(); return *this; }

    //operations
    void rotate(const rot_type& rotation){ transform_.rotate(rotation); }
    void move(const Vec_type& pos){ transform_.move(pos); }
    void scale(const Vec_type& pscale){ transform_.scale(pscale); }
    void moveOrigin(const Vec_type& m) { transform_.moveOrigin(m); };

    void rotation(const rot_type& rotation){ transform_.rotation(rotation); }
    void position(const Vec_type& pos){ transform_.position(pos); }
    void scaling(const Vec_type& pscale){ transform_.scaling(pscale); }
    void origin(const Vec_type& origin){ transform_.origin(origin); }

    const rot_type& rotation() const { return transform_.rotation(); }
    const Vec_type& position() const { return transform_.position(); }
    const Vec_type& scaling() const { return transform_.scaling(); }
    const Vec_type& origin() const { return transform_.origin(); }

    void copyTransform(const transform_type& trans) { transform_ = trans; }
    void copyTransform(const transformable<dim, prec>& trans) { transform_ = trans.transform_; }

    //todo
    const mat_type& transformMatrix() const { return transform_.matrix(); }
    const transform_type& transformObject() const { return transform_; }

	virtual rect_type extents() const { return rect_type{}; };
};

#include <nytl/bits/transform.inl>

}
