#include <nytl/nytl.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>
#include <nytl/transform.hpp>
#include <nytl/serialize.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
using namespace nytl;
/*
constexpr std::size_t multipleAxis(std::size_t D)
{
	return D; //TODO
}

template<std::size_t D, typename P>
class Bivec
{
public:
	Vec<rotationAxis(D), P> simple_ {};
	//Vec<multipleAxis(D), P> multiple_ {};
};

template<std::size_t D, typename P>
class Rotor
{
public:
	Bivec<D, P> bivec_ {};
	float scalar_ {};

	Rotor inverse()
	{
		Rotor ret {};
		ret.bivec_.simple_ = -bivec_.simple_;
		ret.scalar_ = scalar_;
		return ret;
	}
};

template<std::size_t D, typename P>
Rotor<D, P> vecMult(const Vec<D, P>& a, const Vec<D, P>& b)
{
	auto ret = Rotor<D, P> {};
	for(std::size_t i(0); i < D; ++i)
	{
		ret.scalar_ += a[i] * b[i];
	}

	std::size_t idx = 0;
	for(std::size_t i1(0); i1 < a.size() - 1; ++i1)
	{
		for(std::size_t i2(i1 + 1); i2 < b.size(); ++i2)
		{
			ret.bivec_.simple_[idx++] = (a[i1] * b[i2]) - (a[i2] * b[i1]);
		}
	}

	return ret;
}

template<std::size_t D, typename P>
Rotor<D, P> createRotor(const Vec<rotationAxis(D), P>& vec, float angle)
{
	auto ret = Rotor<D, P> {};

	ret.scalar_ = std::cos(radians(angle / 2));
	for(std::size_t i(0); i < D; ++i)
	{
		ret.bivec_.simple_[i] = std::sin(radians(angle / 2));
	}

	return ret;
}

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

template<std::size_t D, typename P>
Vec<D, P> applyRotor(const Vec<D, P>& vec, const Rotor<D, P>& rot)
{
	auto rotMat = identityMat<D>();
	
	for(std::size_t i(0); i < rotationAxis(D); ++i)
	{
		auto planeMat = identityMat<D>();
		auto idx = indexPlane(D, i);

		auto c = std::cos(rot.scalar_ * rot.bivec_.simple_[i]);
		auto s = std::sin(rot.scalar_ * rot.bivec_.simple_[i]);

		planeMat[idx[0]][idx[0]] = c;
		planeMat[idx[0]][idx[1]] = -s;
		planeMat[idx[1]][idx[0]] = s;
		planeMat[idx[1]][idx[1]] = c;

		rotMat *= planeMat;
		std::cout << idx << "\n";
		std::cout << planeMat << "\n";
	}	

	//rotMat *= rot.scalar_;
	std::cout << rotMat << "\n";

	return rotMat * vec;
}

Vec2f rotate(const Vec2f& vec, float value)
{
	Vec2f rot1(std::cos(-value / 2), std::sin(-value / 2));
	Vec2f rot2(std::cos(value / 2), std::sin(value / 2));

	return {std::cos(value) * vec.x - std::sin(value) * vec.y, 
			std::sin(value) * vec.x + std::cos(value) * vec.y};
}
*/

template<typename S> class Printer;

template<char... C>
class Printer<TemplateString<C...>>
{
public:
	static void print()
	{
		nytl::printVars(std::cout, sizeof...(C), "\n", C..., "\n");
	}
};

class BaseClass : public nytl::Serialized<BaseClass, TS_("BaseClass")>
{
};

class DerivedClass : public nytl::DeriveSerialized<BaseClass, DerivedClass, TS_("DerivedClass")>
{
};

template<typename T>
class DerivedTC : public 
	nytl::DeriveSerialized<BaseClass, DerivedTC<T>, TS_("DerivedTC<", typeName<T>(), ">")>
{
};

int main()
{
	constexpr auto str = makeMultipleConstString("hai", "yo", " cooo");

	std::cout << str.size() << "\n";
	for(std::size_t i(0); i < str.size(); ++i)	
		std::cout << str[i] << "\n";

	Printer<TS_("HEllo WW", str, " --df")>::print();

	std::cout << DerivedTC<DerivedClass>::typeName() << "\n";

	return 1;
}

