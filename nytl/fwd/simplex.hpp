// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_SIMPLEX
#define NYTL_INCLUDE_FWD_SIMPLEX

#include <cstdlib> // std::size_t
#include <type_traits> // std::enable_if_t

namespace nytl {

template<std::size_t D, typename P = double, std::size_t A = D>
class Simplex;

template<std::size_t D, typename P = double>
using Line = Simplex<D, P, 1>;

template<std::size_t D, typename P = double>
using Triangle = Simplex<D, P, 2>;

template<std::size_t D, typename P = double>
using Tetrahedron = Simplex<D, P, 3>;

}

#endif //header guard
