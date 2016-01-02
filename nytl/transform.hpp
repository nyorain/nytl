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

//TODO: dont provide specializations for transform, package them in the original class,
//provide specializations for a utility function (e.g. detail::bakeTransformMat or sth) instead.
//Document it then!

#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>
#include <nytl/rect.hpp>
#include <nytl/constants.hpp>

#include <cmath>

namespace nytl
{


//typedefs
template<size_t dim, typename prec = float> class transformable; //only for 2d, todo: 3d
template<size_t dim, typename prec = float> class transform;

using transformable2 = transformable<2, float>;
using transformable3 = transformable<3, float>;

using transform2 = transform<2, float>;
using transform3 = transform<3, float>;

template<typename prec>
class transform<2, prec>
{
public:
    static constexpr const size_t dim = 2;
	using vec_type = vec<dim, prec>;
	using mat_type = squareMat<dim + 1, prec>;
	using rot_type = prec;
	using rect_type = rect<dim, prec>;

protected:
    rot_type rotation_ {0};
    vec_type scaling_ {1, 1};
    vec_type position_ {0, 0};
	vec_type origin_ {0, 0};

	mutable mat_type matrix_ {};
	mutable bool matValid_ {1};

	void invMat() const { matValid_ = 0; }
	void bakeMat() const
	{
	    float rotCos = std::cos(rotation_ * cDeg);
	    float rotSin = std::sin(rotation_ * cDeg);

	    matrix_[0][0] = scaling_.x * rotCos;
	    matrix_[0][1] = scaling_.x * rotSin;
        matrix_[0][2] = -(origin_.x * matrix_[0][0]) - (origin_.y * matrix_[0][1]) + position_.x;
	    matrix_[1][0] = -scaling_.x * rotSin;
	    matrix_[1][1] = scaling_.y * rotCos;
	    matrix_[1][2] = -(origin_.x * matrix_[1][0]) - (origin_.y * matrix_[1][1]) + position_.y;
		matrix_[2][0] = 0;
		matrix_[2][1] = 0;
		matrix_[2][2] = 1;

        matValid_ = 1;
	}

public:
    transform() noexcept : matrix_(identityMat<dim + 1, prec>()) {} 
    transform(const vec_type& pos) noexcept : position_(pos), matValid_(0),
		matrix_(identityMat<dim + 1, prec>())  {}
    ~transform() noexcept = default;

    //operations
    void rotate(const rot_type& rotation){ rotation_ += rotation; invMat(); }
    void move(const vec_type& pos){ position_ += pos; invMat(); }
    void scale(const vec_type& pscale){ scaling_ *= pscale; invMat(); }
    void moveOrigin(const vec_type& m) { origin_ += m; invMat(); };

    void rotation(const rot_type& rotation){ rotation_ = rotation; invMat(); }
    void position(const vec_type& pos){ position_ = pos; invMat(); }
    void scaling(const vec_type& pscale){ scaling_ = pscale; invMat(); }
    void origin(const vec_type& origin) { origin_ = origin; invMat(); }

    const rot_type& rotation() const { return rotation_; }
    const vec_type& position() const { return position_; }
    const vec_type& scaling() const { return scaling_; }
    const vec_type& origin() const { return origin_; }

    template<size_t odim, class oprec>
    operator transform<odim, oprec>()
    {
        transform<odim, oprec> ret{};

        ret.position_ = position_;
        ret.origin_ = origin_;
        if(odim >= 1) ret.scaling_[0] = scaling_[0];
        if(odim >= 2) ret.scaling_[1] = scaling_[1], ret.rotation_[0] = rotation_;

        if(matValid_) ret.matrix_ = matrix_;
        else ret.matValid_ = 0;

        return ret;
    }

    //apply
    vec_type apply(const vec_type& org) const 
		{ return vec_type(vec<dim + 1, prec>(org.x, org.y, 1) * matrix()); }

    //getMatrix
    const mat_type& matrix() const { if(!matValid_) bakeMat(); return matrix_; }
};

//3
template<typename prec>
class transform<3, prec>
{
public:
    static constexpr const size_t dim = 3;

	using vec_type = vec<dim, prec>;
	using mat_type = squareMat<dim + 1, prec>;
	using rot_type = vec_type;
	using rect_type = rect<dim, prec>;

protected:
    rot_type rotation_ {};
    vec_type scaling_ {};
    vec_type position_ {};
	vec_type origin_ {};

	mutable mat_type matrix_ {};
	mutable bool matValid_ {1};

	void invMat() const { matValid_ = 0; }
	void bakeMat() const
	{
	    //todo
	    float sinA = std::sin(rotation_[0] * cDeg);
        float cosA = std::cos(rotation_[0] * cDeg);

        float sinB = std::sin(rotation_[1] * cDeg);
        float cosB = std::cos(rotation_[1] * cDeg);

        float sinC = std::sin(rotation_[2] * cDeg);
        float cosC = std::cos(rotation_[2] * cDeg);

        //todo: pre-calculate this
        mat_type trMatrix(cosB * cosC, cosC * sinA * sinB - cosA * sinC, 
					cosA * cosC * sinB + sinA * sinC, position_[0],
                          cosB * sinC, cosA * cosC + sinA * sinB * sinC, 
					-cosC * sinA + cosA * sinB * sinC, position_[1],
                          -sinB, cosB * sinA, cosA * cosB, position_[2],
                          0, 0, 0, 1);

        mat_type sMatrix(scaling_[0], 0, 0, 0,
                         0, scaling_[1], 0, 0,
                         0, 0, scaling_[2], 0,
                         0, 0, 0, scaling_[3]);

        matrix_ = trMatrix * sMatrix;

        matValid_ = 1;
	}

public:
    transform() noexcept : matrix_(identityMat<dim + 1, prec>()) 
		{ scaling_.fill(1); position_.fill(0); origin_.fill(0); rotation_.fill(0); }
    transform(const vec_type& pos) noexcept : position_(pos), matValid_(0) {}
    ~transform() noexcept = default;

    //operations
    void rotate(const rot_type& rotation){ rotation_ += rotation; invMat(); }
    void move(const vec_type& pos){ position_ += pos; invMat(); }
    void scale(const vec_type& pscale){ scaling_ *= pscale; invMat(); }
    void moveOrigin(const vec_type& m) { origin_ += m; invMat(); };

    void rotation(const rot_type& rotation){ rotation_ = rotation; invMat(); }
    void position(const vec_type& pos){ position_ = pos; invMat(); }
    void scaling(const vec_type& pscale){ scaling_ = pscale; invMat(); }
    void origin(const vec_type& origin) { origin_ = origin; invMat(); }

    const rot_type& rotation() const { return rotation_; }
    const vec_type& position() const { return position_; }
    const vec_type& scaling() const { return scaling_; }
    const vec_type& origin() const { return origin_; }

    template<size_t odim, class oprec>
    operator transform<odim, oprec>()
    {
        transform<odim, oprec> ret{};

        ret.position_ = position_;
        ret.origin_ = origin_;
        if(odim >= 1) ret.scaling_[0] = scaling_[0];
        if(odim >= 2) ret.scaling_[1] = scaling_[1], ret.rotation_[0] = rotation_[0];
        if(odim >= 3) ret.scaling_[2] = scaling_[2], ret.rotation_[1] = rotation_[1], 
			ret.rotation_[2] = rotation_[2];

        if(matValid_) ret.matrix_ = matrix_;
        else ret.matValid_ = 0;

        return ret;
    }

    //apply
    vec_type apply(const vec_type& org) const 
		{ return vec_type(vec<dim + 1, prec>(org.x, org.y, org.z, 1) * matrix()); }

    //getMatrix
    const mat_type& matrix() const { if(!matValid_) bakeMat(); return matrix_; }
};



//transformable helper base class
template<size_t dimension, typename prec>
class transformable
{
protected:
    static constexpr const size_t dim = dimension;
    using value_type = prec;

    using transform_type = transform<dim, value_type>;
	using vec_type = typename transform_type::vec_type;
	using mat_type = typename transform_type::mat_type;
	using rot_type = typename transform_type::rot_type;
	using rect_type = typename transform_type::rect_type;

    transform_type transform_;

public:
    transformable() = default;
    transformable(const vec_type& pos) : transform_(pos) {}

    ~transformable() = default;

    template<size_t odim, typename oprec> 
		transformable(const transformable<odim, oprec>& other) : transform_(other.transform()) {}
    template<size_t odim, typename oprec> 
		transformable(const transform<odim, oprec>& trans) : transform_(trans) {}
    template<size_t odim, typename oprec> 
		transformable<dim, prec>& operator=(const transformable<odim, oprec>& other) 
		{ transform_ = other.transform(); return *this; }

    //operations
    void rotate(const rot_type& rotation){ transform_.rotate(rotation); }
    void move(const vec_type& pos){ transform_.move(pos); }
    void scale(const vec_type& pscale){ transform_.scale(pscale); }
    void moveOrigin(const vec_type& m) { transform_.moveOrigin(m); };

    void rotation(const rot_type& rotation){ transform_.rotation(rotation); }
    void position(const vec_type& pos){ transform_.position(pos); }
    void scaling(const vec_type& pscale){ transform_.scaling(pscale); }
    void origin(const vec_type& origin){ transform_.origin(origin); }

    const rot_type& rotation() const { return transform_.rotation(); }
    const vec_type& position() const { return transform_.position(); }
    const vec_type& scaling() const { return transform_.scaling(); }
    const vec_type& origin() const { return transform_.origin(); }

    void copyTransform(const transform_type& trans) { transform_ = trans; }
    void copyTransform(const transformable<dim, prec>& trans) { transform_ = trans.transform_; }

    //todo
    const mat_type& transformMatrix() const { return transform_.matrix(); }
    const transform_type& transformObject() const { return transform_; }

	virtual rect_type extents() const { return rect_type{}; };
};

}
