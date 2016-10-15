// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_FLAGS_HPP
#define NYTL_INCLUDE_FWD_FLAGS_HPP

#include <type_traits>

namespace nytl
{

template<typename T, typename U = std::underlying_type_t<T>> class Flags;

}

#endif //header guard