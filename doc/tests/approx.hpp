#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

namespace test {

constexpr auto defaultEpsilon = 0.000000001;

template<typename T>
class Approx;

// TODO: remove in C++17
/// Creates an Approx object for the given value.
template<typename T>
Approx<T> approx(const T& value, double epsilon = defaultEpsilon);

/// Approx specialization for floating point types.
template<>
class Approx<double> {
public:
	friend bool operator==(double lhs, const Approx& rhs)
	{
		auto max = std::max(std::abs(lhs), std::abs(rhs.value));
		return std::abs(lhs - rhs.value) < rhs.epsilon * max;
	}

	friend bool operator==(const Approx& lhs, double rhs) { return operator==(rhs, lhs); }
	friend bool operator!=(double lhs, const Approx& rhs) { return !operator==(lhs, rhs); }
	friend bool operator!=(const Approx& lhs, double rhs) { return !operator==(lhs, rhs); }

public:
	double value {};
	double epsilon {defaultEpsilon};
};

template<typename T>
class Approx : public Approx<double> {};

/// Approx specialization for nytl::Vec
template<std::size_t I, typename T>
class Approx<nytl::Vec<I, T>> {
public:
	friend bool operator==(const nytl::Vec<I, T>& lhs, const Approx& rhs)
	{
		if(lhs.size() != rhs.value.size())
			return false;

		for(auto i = 0u; i < lhs.size(); ++i)
			if(lhs[i] != approx(rhs.value[i], rhs.epsilon))
				return false;

		return true;
	}

	friend bool operator==(const Approx& lhs, const nytl::Vec<I, T>& rhs)
	{
		return operator==(rhs, lhs);
	}
	friend bool operator!=(const nytl::Vec<I, T>& lhs, const Approx& rhs)
	{
		return !operator==(lhs, rhs);
	}
	friend bool operator!=(const Approx& lhs, const nytl::Vec<I, T>& rhs)
	{
		return !operator==(lhs, rhs);
	}

public:
	nytl::Vec<I, T> value {};
	double epsilon {defaultEpsilon};
};

/// Approx specialization for nytl::Mat
template<std::size_t R, std::size_t C, typename T>
class Approx<nytl::Mat<R, C, T>> {
public:
	friend bool operator==(const nytl::Mat<R, C, T>& lhs, const Approx& rhs)
	{
		if(lhs.size() != rhs.value.size())
			return false;

		for(auto i = 0u; i < lhs.size(); ++i)
			if(lhs[i] != approx(rhs.value[i], rhs.epsilon))
				return false;

		return true;
	}

	friend bool operator==(const Approx& lhs, const nytl::Mat<R, C, T>& rhs)
	{
		return operator==(rhs, lhs);
	}
	friend bool operator!=(const nytl::Mat<R, C, T>& lhs, const Approx& rhs)
	{
		return !operator==(lhs, rhs);
	}
	friend bool operator!=(const Approx& lhs, const nytl::Mat<R, C, T>& rhs)
	{
		return !operator==(lhs, rhs);
	}

public:
	nytl::Mat<R, C, T> value {};
	double epsilon {defaultEpsilon};
};

template<typename T>
Approx<T> approx(const T& value, double epsilon)
{
	return {value, epsilon};
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
	os << "Approx(" << approx.value << ")";
	return os;
}

} // namespace test
