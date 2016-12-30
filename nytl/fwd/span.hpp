// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_SPAN
#define NYTL_INCLUDE_FWD_SPAN

#include <nytl/scalar.hpp> // nytl::constants::dynamicSize
#include <cstdlib> // std::size_t

namespace nytl {
	template<typename T, std::size_t N = constants::dynamicSize> class Span;
}

#endif //header guard
