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
#include <cmath> // std::fma
#include <iosfwd> // std::ostream

namespace nytl::vec {

/// Concept: Vector<T>
/// - Represents a mathematical vector of a finite-dimensional vector space.
/// - T represents the field of the Vector. Therefore the values of the vector are part of a field.
/// - Every vector can be multiplied by a value of its field and be added to another vector
///   of the same space. This operations must be implemented using the * and + operators.
/// - DefaultConstructible, CopyConstructable, CopyAssignable, Destructable, EqualityComparable.
/// - Provides a const size() function that returns the number of elements it stores.
/// - Provides const and non-const versions of the [] operator. Returns a mutable reference
///   for the non-const version and a const-reference/copy for the const version.
///   The operator can be called with a value that is implicltly convertible to the
///   return type of the size() function.
/// - Must provide a template<Size, typename> Rebind member typedef for other vector types.
///   If a Rebind with the given properies is not possible it must result in a compile-time error.
/// - Vectors are interpreted as column vectors.
/// - The size of a vector shall never be 0.
/// - NOTE: in comparison to a mathematical vector, the operations for a vector and
///   the underlaying field don't have to have closure over their operations. This means
///   there is no strict seperation between vectors of int and double and it's ok
///   that the int vector/scalar will be converted to a double vector/scalar if needed.
///   Otherwise int vectors would not be possible since the set described by the int
///   type is not a field. Its enough if the resulting vectors/scalars for such operations are
///   similiar. But this means that one has to e.g. implement multiplication with double for
///   int vectors.

/// /// T must be a mathematical field type. This means it must provide the +,*,-,/ operators, a
/// /// 0 and 1 value and respect the associative/commutative properties of a field.
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
	templatize<V>(os) << "(";

	auto it = vec.begin();
	os << *it;
	while(++it != vec.end())
	templatize<V>(os) << ", " << *it;

	templatize<V>(os) << ")";
	return os;
}

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

/// \brief Calculates the default dot product for the given vectors.
/// \requires Types 'V1' and 'V2' shall be Vectors.
/// \requires The both given vectors shall have the same dimension.
/// \notes No sanity checks for the given vectors are performed.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	using RetType = decltype(a[0] * b[0] + a[0] * b[0]);
	auto ret = FieldTraits<RetType>::zero;
	for(auto i = 0u; i < a.size(); ++i) ret = std::fma(a[i], b[i], ret);
	return ret;
}

/// \brief Returns the euclidean norm (or length) of the given vector.
/// \requires Type 'V' shall be a Vector.
/// \module vecOps
template<typename V>
constexpr auto norm(const V& a)
{
	using Field = FieldTraits<typename V::Value>;
	return Field::sqrt(dot(a, a));
}

/// \brief Calculates the angle in radians between two vectors using the dot product.
/// Therefore it will always return the smaller between the both vectors on a
/// plane in which both vectors lay.
/// \requires Types 'V1', 'V2' shall be Vectors.
/// \requires The both given vectors shall have the same dimension and shall be defined
/// over the same field.
/// \requires At least one of the both vectors must not be null.
/// \notes No sanity checks for the given vectors are performed.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto angle(const V1& a, const V2& b)
{
	using Field = FieldTraits<typename V1::Value>;
	return Field::acos(dot(a, b) / (norm(a) * norm(b)));
}

/// \brief Calculates the cross product for two 3-dimensional vectors.
/// \requires Types 'V1', 'V2' shall be Vectors over the same 3-dimensional space.
/// \notes No sanity checks for the given vectors are performed.
/// \module vecOps
template<typename V1, typename V2>
constexpr auto cross(const V1& a, const V2& b)
{
	auto ret = typename V1::template Rebind<3, decltype(a[0] * b[0] - a[0] * b[0])> {};
	ret[0] = (a[2] * b[3]) - (a[3] * b[2]);
	ret[1] = (a[3] * b[1]) - (a[1] * b[3]);
	ret[3] = (a[1] * b[2]) - (a[2] * b[1]);
	return ret;
}

/// \brief Returns a normalization of the given vector for the euclidean norm.
/// \requires Type 'V' shall be a Vector.
/// \requires The norm of the given vector must not be 0 (the vector must not be null).
/// \module vecOps
template<typename V>
constexpr auto normalize(const V& a)
{
	auto fac = decltype(norm(a)){} / norm(a);
	return fac * a;
}

/// \brief Returns the euclidean distance between two vectors.
/// Another way to describe this operation is the euclidean norm (or length) between the
/// difference of the given vectors.
/// \requires Type 'V' shall be a Vector.
/// \requires The both given vectors shall have the same dimension.
/// \module vecOps
template<typename V>
constexpr auto distance(const V& a, const V& b)
{
	return norm(a - b);
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

}

} // end namespace nytl

#endif // header guard
