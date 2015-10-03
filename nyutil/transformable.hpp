#pragma once

#include <nyutil/mat.hpp>
#include <nyutil/vec.hpp>
#include <nyutil/rect.hpp>
#include <nyutil/constants.hpp>

#include <cmath>

namespace nyutil
{


//typedefs
template<size_t dim, typename prec = float> class transformable; //only for 2d, todo: 3d
template<size_t dim, typename prec = float> class transform;

using transformable2 = transformable<2, float>;
using transformable3 = transformable<3, float>;

using transform2 = transform<2, float>;
using transform3 = transform<3, float>;

//transform
//2
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
    rot_type rotation_ {};
    vec_type scale_ {};
    vec_type position_ {};
	vec_type origin_ {};

	mutable mat_type matrix_ {};
	mutable bool matValid_ {1};

	void invMat() const { matValid_ = 0; }
	void bakeMat() const
	{
	    float rotCos = std::cos(rotation_ * cDeg);
	    float rotSin = std::sin(rotation_ * cDeg);

	    matrix_[0][0] = scale_.x * rotCos;
	    matrix_[0][1] = scale_.x * rotSin;
        matrix_[0][2] = -(origin_.x * matrix_[0][0]) - (origin_.y * matrix_[0][1]) + position_.x;
	    matrix_[1][0] = -scale_.x * rotSin;
	    matrix_[1][1] = scale_.y * rotCos;
	    matrix_[1][2] = -(origin_.x * matrix_[1][0]) - (origin_.y * matrix_[1][1]) + position_.y;

        matValid_ = 1;
	}

public:
    transform() : matrix_(identityMat<dim + 1, prec>()) { scale_.fill(1); position_.fill(0); origin_.fill(0); }
    ~transform() noexcept = default;

    //operations
    void rotate(const rot_type& rotation){ rotation_ += rotation; invMat(); }
    void move(const vec_type& pos){ position_ += pos; invMat(); }
    void scale(const vec_type& pscale){ scale_ *= pscale; invMat(); }
    void moveOrigin(const vec_type& m) { origin_ += m; invMat(); };

    void setRotation(const rot_type& rotation){ rotation_ = rotation; invMat(); }
    void setPosition(const vec_type& pos){ position_ = pos; invMat(); }
    void setScale(const vec_type& pscale){ scale_ = pscale; invMat(); }
    void setOrigin(const vec_type& origin) { origin_ = origin; invMat(); }

    const rot_type& getRotation() const { return rotation_; }
    const vec_type& getPosition() const { return position_; }
    const vec_type& getScale() const { return scale_; }
    const vec_type& getOrigin() const { return origin_; }

    template<size_t odim, class oprec>
    operator transform<odim, oprec>()
    {
        transform<odim, oprec> ret{};

        ret.position_ = position_;
        ret.origin_ = origin_;
        if(odim >= 1) ret.scale_[0] = scale_[0];
        if(odim >= 2) ret.scale_[1] = scale_[1], ret.rotation_[0] = rotation_;

        if(matValid_) ret.matrix_ = matrix_;
        else ret.matValid_ = 0;

        return ret;
    }

    //apply
    vec_type apply(const vec_type& org) const { return vec_type(vec<dim + 1, prec>(org.x, org.y, 1) * getMatrix()); }
    rect_type apply(const rect_type& org) const { return rect_type(apply(org.position), apply(org.size)); }

    //getMatrix
    const mat_type& getMatrix() const { if(!matValid_) bakeMat(); return matrix_; }
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
    vec_type scale_ {};
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
        mat_type trMatrix(cosB * cosC, cosC * sinA * sinB - cosA * sinC, cosA * cosC * sinB + sinA * sinC, position_[0],
                          cosB * sinC, cosA * cosC + sinA * sinB * sinC, -cosC * sinA + cosA * sinB * sinC, position_[1],
                          -sinB,       cosB * sinA,                      cosA * cosB,                       position_[2],
                          0,           0,                                0,                                 1);

        mat_type sMatrix(scale_[0], 0, 0, 0,
                         0, scale_[1], 0, 0,
                         0, 0, scale_[2], 0,
                         0, 0, 0, scale_[3]);

        matrix_ = trMatrix * sMatrix;

        matValid_ = 1;
	}

public:
    transform() : matrix_(identityMat<dim + 1, prec>()) { scale_.fill(1); position_.fill(0); origin_.fill(0); rotation_.fill(0); }
    ~transform() noexcept = default;

    //operations
    void rotate(const rot_type& rotation){ rotation_ += rotation; invMat(); }
    void move(const vec_type& pos){ position_ += pos; invMat(); }
    void scale(const vec_type& pscale){ scale_ *= pscale; invMat(); }
    void moveOrigin(const vec_type& m) { origin_ += m; invMat(); };

    void setRotation(const rot_type& rotation){ rotation_ = rotation; invMat(); }
    void setPosition(const vec_type& pos){ position_ = pos; invMat(); }
    void setScale(const vec_type& pscale){ scale_ = pscale; invMat(); }
    void setOrigin(const vec_type& origin) { origin_ = origin; invMat(); }

    const rot_type& getRotation() const { return rotation_; }
    const vec_type& getPosition() const { return position_; }
    const vec_type& getScale() const { return scale_; }
    const vec_type& getOrigin() const { return origin_; }

    template<size_t odim, class oprec>
    operator transform<odim, oprec>()
    {
        transform<odim, oprec> ret{};

        ret.position_ = position_;
        ret.origin_ = origin_;
        if(odim >= 1) ret.scale_[0] = scale_[0];
        if(odim >= 2) ret.scale_[1] = scale_[1], ret.rotation_[0] = rotation_[0];
        if(odim >= 3) ret.scale_[2] = scale_[2], ret.rotation_[1] = rotation_[1], ret.rotation_[2] = rotation_[2];

        if(matValid_) ret.matrix_ = matrix_;
        else ret.matValid_ = 0;

        return ret;
    }

    //apply
    vec_type apply(const vec_type& org) const { return vec_type(vec<dim + 1, prec>(org.x, org.y, org.z, 1) * getMatrix()); }
    rect_type apply(const rect_type& org) const { return rect_type(apply(org.position), apply(org.size)); }

    //getMatrix
    const mat_type& getMatrix() const { if(!matValid_) bakeMat(); return matrix_; }
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
    ~transformable() = default;

    template<size_t odim, typename oprec> transformable(const transformable<odim, oprec>& other) : transform_(other.getTransform()) {}
    template<size_t odim, typename oprec> transformable(const transform<odim, oprec>& trans) : transform_(trans) {}
    template<size_t odim, typename oprec> transformable<dim, prec>& operator=(const transformable<odim, oprec>& other) { transform_ = other.getTransform(); return *this; }

    //operations
    void rotate(const rot_type& rotation){ transform_.rotate(rotation); }
    void move(const vec_type& pos){ transform_.move(pos); }
    void scale(const vec_type& pscale){ transform_.scale(pscale); }
    void moveOrigin(const vec_type& m) { transform_.moveOrigin(m); };

    void setRotation(const rot_type& rotation){ transform_.setRotation(rotation); }
    void setPosition(const vec_type& pos){ transform_.setPosition(pos); }
    void setScale(const vec_type& pscale){ transform_.setScale(pscale); }
    void setOrigin(const vec_type& origin){ transform_.setOrigin(origin); }

    const rot_type& getRotation() const { return transform_.getRotation(); }
    const vec_type& getPosition() const { return transform_.getPosition(); }
    const vec_type& getScale() const { return transform_.getScale(); }
    const vec_type& getOrigin() const { return transform_.getOrigin(); }

    void copyTransform(const transform_type& trans) { transform_ = trans; }
    void copyTransform(const transformable<dim, prec>& trans) { transform_ = trans.transform_; }

    //todo
    const mat_type& getTransformMatrix() const { return transform_.getMatrix(); }
    const transform_type& getTransform() const { return transform_; }

	virtual rect_type getExtents() const { return rect_type{}; };
	rect_type getTransformedExtents() const { return transform_.apply(getExtents()); }
};

}
