// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_SIMPLEX_HPP
#define NYTL_INCLUDE_FWD_SIMPLEX_HPP

#include <cstdint>

namespace nytl
{

template<std::size_t D, typename P = float, std::size_t A = D> class Simplex;
template<std::size_t D, typename P = float> using Line = Simplex<D, P, 1>;
template<std::size_t D, typename P = float> using Triangle = Simplex<D, P, 2>;
template<std::size_t D, typename P = float> using Tetrahedron = Simplex<D, P, 3>;

}

#endif //header guard
