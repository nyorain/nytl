// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_FLAGS
#define NYTL_INCLUDE_FWD_FLAGS

#include <type_traits> // std::underlaying_type_t

namespace nytl {
	template<typename T, typename U = std::underlying_type_t<T>> class Flags;
}

#endif //header guard
