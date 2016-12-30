#pragma once

#include <nytl/tmpUtil.hpp>
#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

#include <type_traits>
#include <limits>
#include <cmath>
#include <iostream>

int failed;

#define CHECK_EXPECT(expression, expect) { \
		auto&& ce_a = expression; \
		auto&& ce_b = expect; \
		if(!testEqual(ce_a, ce_b)) \
			checkExpectFailed(__FILE__, __LINE__, #expression, #expect, ce_a, ce_b); \
	}

#define CHECK_ERROR(expression, error) { \
		bool thrown {}; \
		bool sthelse {}; \
		try{ expression; } \
		catch(const error&) { thrown = true; } \
		catch(...) { sthelse = true; } \
		if(!thrown) checkErrorFailed(__FILE__, __LINE__, #expression, #error, sthelse); \
	}

template<typename T, typename = void>
struct Printable {
	static const char* call(const T&)
	{
		static auto txt = std::string("<not printable : ") + typeid(T).name() + std::string(">");
		return txt.c_str();
	}
};

template<typename T>
struct Printable<T, nytl::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> {
	static const auto& call(const T& obj) { return obj; }
};

template<typename T>
decltype(auto) printable(const T& obj) { return Printable<T>::call(obj); }

template<typename A, typename B>
void checkExpectFailed(const char* file, int line, const char* expression,
	const char* expect, const A& a, const B& b)
{
	std::cout << "check expect failed in " << file << ":" << line << ":\n    <"
			  << expression << "> = <" << printable(a) << "> instead of\n    <"
			  << expect << "> = <" << printable(b) << ">\n"
			  << "===================================================================\n";
	++failed;
}

void checkErrorFailed(const char* file, int line, const char* expression, const char* error,
	bool otherError)
{
	std::cout << "check error failed in " << file << ":" << line << ":\n    <"
			  << expression << "> did not throw error <" << error << ">\n";
	if(otherError) std::cout << "    Other error was thrown instead!\n";
	std::cout << "===================================================================\n";

	++failed;
}

template<typename T, typename = void>
struct Equal {
	constexpr static bool call(const T& a, const T& b) { return a == b; }
};

template<typename T>
constexpr bool almostEqual(T a, T b, T epsilon = 0.0000001)
{
	return std::abs(a - b) < epsilon;

	// alternatively, for smaller errors using ulp:
	// return std::abs(a - b) < std::abs(a + b) * std::numeric_limits<T>::epsilon() * ulp
	// 	|| std::abs(a - b) < std::numeric_limits<T>::min();
}

template<typename T>
using EnableIfFloatingPoint = nytl::void_t<std::enable_if_t<!std::numeric_limits<T>::is_integer>,
	decltype(std::abs(std::declval<T>()))>;

template<> struct Equal<float> {
	constexpr static bool call(float a, float b) { return almostEqual(a, b); }
};

template<> struct Equal<double> {
	static bool call(double a, double b) { return almostEqual(a, b); }
};

template<typename A, typename B> constexpr
bool testEqual(const A& a, const B& b)
{
	return Equal<std::common_type_t<A, B>>::call(a, b);
}

// better equal function for floating point vecs and mats
template<typename T, std::size_t D>
struct Equal<nytl::Vec<D, T>> {
	static constexpr bool call(const nytl::Vec<D, T>& a, const nytl::Vec<D, T>& b)
	{
		for(auto i = 0u; i < a.size(); ++i)
			if(!testEqual(a[i], b[i])) return false;
		return true;
	}
};

template<typename T, std::size_t R, std::size_t C>
struct Equal<nytl::Mat<T, R, C>> : public Equal<nytl::Vec<R, nytl::Vec<C, T>>> {};
