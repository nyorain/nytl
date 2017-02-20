// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various vector related operations.

#pragma once

#ifndef NYTL_INCLUDE_VEC_OPS
#define NYTL_INCLUDE_VEC_OPS

#include <nytl/field.hpp> // nytl::FieldTraits
#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/scalar.hpp> // nytl::accumulate

#include <functional> // std::plus, std::multiplies
#include <stdexcept> // std::invalid_argument
#include <cmath> // std::fma
#include <iosfwd> // std::ostream

namespace nytl::vec {

/// Concept: Vector<T>
/// - Represents a mathematical vector of a finite-dimensional vector space.
/// - T represents the field of the Vector. Therefore the values of the vector are part of a field.
/// - Every vector can be multiplied by a value of its field and be added to another vector
///   of the same space. This operations must be implemented using the * and + operators.
/// - DefaultConstructible, CopyConstructable, CopyAssignable, Destructable, EqualityComparable.
/// - Provides a const size() function that returns the number of elements it stores aka
///   the dimension this vector has.
/// - Provides const and non-const versions of the [] operator. Returns a mutable reference
///   for the non-const version and a const-reference/copy for the const version.
///   The operator can be called with an integer value that can hold the size of the vector.
/// - Must provide a template<Size, typename> Rebind member typedef for other vector types.
///   If a Rebind with the given properies is not possible it must result in a compile-time error.
/// - Vectors are interpreted as column vectors regarding matrix operations.
/// - The size (or dimension) of a vector shall never be 0.
/// - There must be a valid nytl::FieldTraits specialization for the field over which
///   the vector is defined.

///   NOTE: in comparison to a mathematical vector, the operations for a vector and
///     the underlaying field don't have to have closure over their operations. This means
///     there is no strict seperation between vectors of int and double and it's ok
///     that the int vector/scalar will be converted to a double vector/scalar if needed.
///     Otherwise int vectors would not be possible since the set described by the int
///     type is not a field. Its enough if the resulting vectors/scalars for such operations are
///     similiar. But this means that one has to e.g. implement multiplication with double for
///     int vectors.

/// T must be a mathematical field type. This means it must provide the +,*,-,/ operators, a
/// 0 and 1 value and respect the associative/commutative properties of a field.

/// template<...>
/// class Vector {
/// public:
/// 	using Size = ...; // usually std::size_t. Must be convertible from/to int.
/// 	using Value = ...; // the value type, mathematical field
/// 	using Reference = ...; // usually Value&
/// 	using ConstReference = ...; // usually const Value&
///
/// 	// rebinds the vector implementation class
/// 	template<Size Dim, typename T> using Rebind = ...;
///
/// 	static constexpr Size dim = ...; // dimension the vector has. Might be a symbolic value.
///		Vector create(Size size); // creates a vector with the given size
///
/// public:
/// 	Vector();
/// 	~Vector();
/// 	Vector(const Vector<T>&);
/// 	Vector& operator=(const Vector<T>&);
///
/// 	Size size() const;
///
/// 	Reference operator[](Size);
/// 	ConstReference operator[](Size) const;
///
/// 	// Additionally all iterator typedefs and functions as in stl containers
/// };
///
/// // The first operations must return a vector with the same dimension and over a similiar field.
/// auto operator*(Value, Vector);
/// auto operator+(Vector, Vector);
/// auto operator-(Vector, Vector);
/// bool operator==(Vector, Vector);
/// bool operator!=(Vector, Vector);
///
/// For an example Vector implementation: nytl/vec.hpp [nytl::Vec]().

// fwd-declaration
template<typename V>
constexpr auto length(const V& a);

/// \brief Prints the given vector to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types.
/// \requires Type 'V' shall be a Vector
/// \requires There must be an implementation of operator<<(std::ostream&, V::Value).
/// \module vecOps
template<typename V>
std::ostream& print(std::ostream& os, const V& vec)
{
	auto& tos = templatize<V>(os); // we don't want to include ostream
	tos << "(";

	auto it = vec.begin();
	tos << *it;
	while(++it != vec.end())
	tos << ", " << *it;

	tos << ")";
	return os;
}

namespace detail {

// TODO: c++17!

/// \brief Helper that asserts that the given vectors have the same dimension.
/// \requires Types 'V1','V2' shall be Vector types.
template<typename V1, typename V2>
struct AssertSameDimensions {
	static constexpr void call(const V1& a, const V2& b)
	{
		if(a.size() != b.size())
			throw std::invalid_argument("nytl::vec: vectors must have same dimension");
	}
};

template<typename V1, unsigned int Dim>
struct AssertDimension {
	static constexpr void call(const V1& a)
	{
		if(a.size() != Dim)
			throw std::invalid_argument("nytl::vec: vector must have specified dimension");
	}
};

/// \brief Asserts that the both given vectors have the same dimension.
template<typename V1, typename V2>
constexpr void assertSameDimensions(const V1& a, const V2& b)
{
	AssertSameDimensions<V1, V2>::call(a, b);
}

template<unsigned int Dim, typename V>
constexpr void assertDimension(const V& a)
{
	AssertDimension<V, Dim>::call(a);
}

} // namespace detail

/// \brief Sums up all values of the given vector using the + operator.
/// \requires Type 'V' shall be a Vector
/// \module vecOps
template<typename V>
constexpr auto sum(const V& a)
{
	auto zero = FieldTraits<typename V::Value>::zero;
	return accumulate(a.begin(), a.end(), zero, std::plus<>());
}

/// \brief Mutliplies all values of the given vector using the * operator.
/// \requires Type 'V' shall be a non-empty Vector
/// \module vecOps
template<typename V>
constexpr auto multiply(const V& a)
{
	auto one = a[0];
	return accumulate(a.begin() + 1, a.end(), one, std::multiplies<>());
}

/// \brief Returns the euclidean distance between two vectors.
/// Another way to describe this operation is the length between the
/// difference of the given vectors.
/// \requires Types 'V1','V2' shall be Vector types.
/// \requires The both given vectors shall have the same dimension.
/// Will not check for this but subtract them from each other.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto distance(const V1& a, const V2& b)
{
	return length(a - b);
}

// Vec operations without argument checking
namespace nocheck {

/// Like dot, but no sanity checks are performed.
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	using RetType = decltype(a[0] * b[0] + a[0] * b[0]);
	auto ret = FieldTraits<RetType>::zero;
	for(auto i = 0u; i < a.size(); ++i) ret = std::fma(a[i], b[i], ret);
	return ret;
}

/// Like angle, but no sanity checks are performed.
/// This will also not handle the special case that both vectors are equal.
template<typename V1, typename V2>
constexpr auto angle(const V1& a, const V2& b)
{
	using Field = FieldTraits<typename V1::Value>;
	return Field::acos(nocheck::dot(a, b) / (length(a) * length(b)));
}

/// Like cross, but no sanity checks are performed.
template<typename V1, typename V2>
constexpr auto cross(const V1& a, const V2& b)
{
	auto ret = V1::template Rebind<3, decltype(a[0] * b[0] - a[0] * b[0])>::create(3);
	ret[0] = (a[1] * b[2]) - (a[2] * b[1]);
	ret[1] = (a[2] * b[0]) - (a[0] * b[2]);
	ret[2] = (a[0] * b[1]) - (a[1] * b[0]);
	return ret;
}

// Like normalize, but no sanity checks are performed.
template<typename V>
constexpr auto normalize(const V& a)
{
	using Field = FieldTraits<typename V::Value>;
	return (Field::one / length(a)) * a;
}

} // namespace nocheck

/// \brief Calculates the default dot product for the given vectors.
/// \requires Types 'V1' and 'V2' shall be Vectors.
/// \throws std::invalid_argument if the size of the input vectors differs.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);
	return nocheck::dot(a, b);
}

/// \brief Calculates the angle in radians between two vectors using the dot product.
/// Therefore it will always return the smaller between the both vectors on a
/// plane in which both vectors lay.
/// For two equal vectors, it will return 0.0.
/// \requires Types 'V1', 'V2' shall be Vectors.
/// \throws std::invalid_argument if the size of the input vectors differs.
/// \throws std::domain_error if at lesat one of the given vectors has a length of 0.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto angle(const V1& a, const V2& b)
{
	using Field = FieldTraits<typename V1::Value>;
	detail::assertSameDimensions(a, b);

	if(length(a) == 0 || length(b) == 0)
		throw std::domain_error("nytl::vec::angle: both vectors are null");

	if(a == b)
		return Field::acos(Field::one);

	return nocheck::angle(a, b);
}

/// \brief Calculates the cross product for two 3-dimensional vectors.
/// \requires Types 'V1', 'V2' shall be Vectors over the same 3-dimensional space.
/// \throws std::domain_error if at least on of the input vectors does not have a size of 3.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto cross(const V1& a, const V2& b)
{
	detail::assertDimension<3>(a);
	detail::assertDimension<3>(b);

	return nocheck::cross(a, b);
}

/// \brief Returns a normalization of the given vector for the euclidean norm.
/// \requires Type 'V' shall be a Vector.
/// \throws std::domain_error if the vector has the length 0.
/// \module vecOps
template<typename V>
constexpr auto normalize(const V& a)
{
	if(length(a) == 0)
		throw std::domain_error("nytl::vec::normalize: vector has length 0");

	return nocheck::normalize(a);
}

/// \brief Returns the euclidean norm (or length) of the given vector.
/// \requires Type 'V' shall be a Vector.
/// \module vecOps
template<typename V>
constexpr auto length(const V& a)
{
	using Field = FieldTraits<typename V::Value>;
	return Field::sqrt(nocheck::dot(a, a));
}

/// Contains various component-wise operations for Vectors.
namespace cw {

/// \brief Returns a vector holding the component-wise maximum of the given Vectors.
/// \requires Type 'V' shall be a Vector type.
/// \requires The both given vectors shall have the same dimension.
/// \module vecOps
template<typename V>
constexpr auto max(V a, const V& b)
{
	for(auto i = 0u; i < a.size(); ++i)
		if(b[i] > a[i])
			a[i] = b[i];
	return a;
}

/// \brief Returns a vector holding the component-wise maximum of the given Vectors.
/// \requires Type 'V' shall be a Vector type.
/// \requires The both given vectors shall have the same dimension.
/// \module vecOps
template<typename V>
constexpr auto min(V a, const V& b)
{
	for(auto i = 0u; i < a.size(); ++i)
		if(b[i] < a[i])
			a[i] = b[i];
	return a;
}

/// \brief Multiplies the two vectors component wise
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto multiply(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);

	auto ret = typename V1::template Rebind<V1::dim, decltype(a[0] * b[0])> {};
	for(auto i = 0u; i < a.size(); ++i)
		ret[i] = a[i] * b[i];
	return ret;
}

/// \brief Component-wise divides the first vector by the second one.
/// Will not perform any zero checks.
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto divide(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);

	auto ret = typename V1::template Rebind<V1::dim, decltype(a[0] / b[0])> {};
	for(auto i = 0u; i < a.size(); ++i)
		ret[i] = a[i] / b[i];
	return ret;
}

} // namespace cw
} // namespace nytl::vec

#endif // header guard
