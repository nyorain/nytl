#pragma once

#include "bugged.hpp"
#include <nytl/approx.hpp>
#include <ostream>

namespace nytl {

template<typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
	os << "Approx(" << approx.value << ")";
	return os;
}

} // namespace nytl
