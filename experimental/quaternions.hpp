#pragma once

#include <nytl/vec.hpp>

namespace nytl
{

/*
///\ingroup math
template<typename P>
class Quat
{
public:
	using QuatType = Quat<P>;
	using VecType = Vec<3, P>;

public:
	VecType axis_;
	P angle_;

public:
	Quat() noexcept = default;
	explicit Quat(const VecType& euler);

	template<typename OP> Quat& operator*()(const OP& other);
	template<typename OP> Quat& operator/()(const OP& other);

	template<typename OP> Quat& operator*()(const Quat<OP>& other);
	template<typename OP> Quat& operator+()(const Quat<OP>& other);
	template<typename OP> Quat& operator-()(const Quat<OP>& other);
};
*/

constexpr std::size_t pow(std::size_t base, std::size_t exp)
{
	return (exp > 1) ? pow(base, exp - 1) * base : base;
}


///\ingroup math
///Generalized quaternion appliable for all dimensions.
template<std::size_t D, typename P>
class Rotor
{
public:
	Vec<pow(2, D - 1), P> values_;

public:
	template<typename OP> Rotor& operator*(const OP& other);
	template<typename OP> Rotor& operator/(const OP& other);

	template<typename OP> Rotor& operator*(const Rotor<D, OP>& other);
	template<typename OP> Rotor& operator+(const Rotor<D, OP>& other);
	template<typename OP> Rotor& operator-(const Rotor<D, OP>& other);
};

template<std::size_t D, typename P>
template<typename OP> Rotor<D, P>& Rotor<D, P>::operator*(const OP& other)
{
	values_ *= other;
	return *this;
}

template<std::size_t D, typename P>
template<typename OP> Rotor<D, P>& Rotor<D, P>::operator/(const OP& other)
{
	values_ /= other;
	return *this;
}

template<std::size_t D, typename P>
template<typename OP> Rotor<D, P>& Rotor<D, P>::operator*(const Rotor<D, OP>& other)
{
	return *this;
}
//#include <nytl/bits/quaternion.inl>

}
