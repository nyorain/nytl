#pragma once

#include <nyutil/mat.hpp>
#include <nyutil/vec.hpp>
#include <nyutil/rect.hpp>

#include <cmath>

namespace nyutil
{

constexpr const double cPi = 3.14159265359;
constexpr const double cDeg = cPi / 180.0;

template<size_t dim, typename prec = float> class transformable; //only for 2d, todo: 3d
template<size_t dim, typename prec = float> class transform;

//transform
template<typename prec>
class transform<2, prec>
{
protected:
    static constexpr const size_t dim = 2;
	using pVec = vec<dim, prec>;
	using pMat = squareMat<dim + 1, prec>;
	using pRect = rect<dim, prec>;

    prec rotation_ {0}; //todo, must be vector, too?
    pVec scale_ {};
    pVec position_ {};
	pVec origin_ {}; //relative origin of the object.

	mutable pMat matrix_ {};
	mutable bool matValid_ {0}; //stores if the cached matrix_ is valid

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

	    /*
        pMat posMat = identityMat<dim + 1, prec>();
        posMat[0][2] = position_.x;
        posMat[1][2] = position_.y;

        pMat rotMat = identityMat<dim + 1, prec>();
        rotMat[0][0] = rotCos;
        rotMat[0][1] = rotSin;
        rotMat[1][1] = -rotSin;
        rotMat[1][1] = rotCos;

        pMat scaleMat = identityMat<dim + 1, prec>();
        scaleMat[0][0] = scale_.x;
        scaleMat[1][1] = scale_.y;

        matrix_ = posMat * rotMat * scaleMat;
        */

        matValid_ = 1;

	}

public:
    transform() : matrix_(identityMat<dim + 1, prec>()) { scale_.setAllTo(1); position_.setAllTo(0); origin_.setAllTo(0); }
    ~transform() = default;

    //copy constructor
    template<size_t odim, class oprec> transform(const transform<odim, oprec>& other) :
        rotation_(other.rotation_), scale_(other.scale_), position_(other.position_), origin_(other.origin_)
    {
        if(other.matValid_)
        {
            matrix_ = other.matrix_;
            matValid_ = 1;
        }
    };

    //ass. operator
    template<size_t odim, class oprec> transform<dim, prec>& operator=(const transform<odim, oprec>& other)
    {
        rotation_ = other.rotation_;
        scale_ = other.scale_;
        position_ = other.position_;
        origin_ = other.origin_;
        if(other.matValid_)
        {
            matrix_ = other.matrix_;
            matValid_ = 1;
        }
    }

    //operations
    void rotate(const prec& rotation){ rotation_ += rotation; invMat(); }
    void move(const pVec& pos){ position_ += pos; invMat(); }
    void scale(const pVec& pscale){ scale_ *= pscale; invMat(); }
    void moveOrigin(const pVec& m) { origin_ += m; };

    void setRotation(const prec& rotation){ rotation_ = rotation; invMat(); }
    void setPosition(const pVec& pos){ position_ = pos; invMat(); }
    void setScale(const pVec& pscale){ scale_ = pscale; invMat(); }
    void setOrigin(const pVec& origin) { origin_ = origin; }

    const prec& getRotation() const { return rotation_; }
    const pVec& getPosition() const { return position_; }
    const pVec& getScale() const { return scale_; }
    const pVec& getOrigin() const { return origin_; }

    //apply
    pVec apply(const pVec& org) const { return pVec(vec3f(org.x, org.y, 1.f) * getMatrix()); }
    pRect apply(const pRect& org) const { return pRect(apply(org.position), apply(org.size)); }

    //getMatrix
    const pMat& getMatrix() const { if(!matValid_) bakeMat(); return matrix_; }
};

//transformable
template<typename prec>
class transformable<2,prec>
{
protected:
    static constexpr const size_t dim = 2;
	using pVec = vec<dim, prec>;
	using pMat = squareMat<dim + 1, prec>;
	using pRect = rect<dim, prec>;
	using pTrans = transform<dim, prec>;

    pTrans transform_;

public:
    transformable() = default;
    ~transformable() = default;

    template<size_t odim, typename oprec> transformable(const transformable<odim, oprec>& other) : transform_(other.getTransform()) {}
    template<size_t odim, typename oprec> transformable(const transform<odim, oprec>& trans) : transform_(trans) {}
    template<size_t odim, typename oprec> transformable<dim, prec>& operator=(const transformable<odim, oprec>& other) { transform_ = other.getTransform(); return *this; }

    //operations
    void rotate(const prec& rotation){ transform_.rotate(rotation); }
    void move(const pVec& pos){ transform_.move(pos); }
    void scale(const pVec& pscale){ transform_.scale(pscale); }
    void moveOrigin(const pVec& m) { transform_.moveOrigin(m); };

    void setRotation(const prec& rotation){ transform_.setRotation(rotation); }
    void setPosition(const pVec& pos){ transform_.setPosition(pos); }
    void setScale(const pVec& pscale){ transform_.setScale(pscale); }
    void setOrigin(const pVec& origin){ transform_.setOrigin(origin); }

    const prec& getRotation() const { return transform_.getRotation(); }
    const pVec& getPosition() const { return transform_.getPosition(); }
    const pVec& getScale() const { return transform_.getScale(); }
    const pVec& getOrigin() const { return transform_.getOrigin(); }

    void copyTransform(const pTrans& trans) { transform_ = trans; }
    void copyTransform(const transformable<dim, prec>& trans) { transform_ = trans.transform_; }

    //todo
    const pMat& getTransformMatrix() const { return transform_.getMatrix(); }
    const pTrans& getTransform() const { return transform_; }

	virtual pRect getExtents() const = 0;
	pRect getTransformedExtents() const { return transform_.apply(getExtents()); }
};

}
