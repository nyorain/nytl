// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_REFERENCED
#define NYTL_INCLUDE_FWD_REFERENCED

#include <memory> // std::default_delete

namespace nytl {
	template<typename T, typename Deleter = std::default_delete<T>> class Referenced;
	template<typename T> class IntrusivePtr;
}

#endif //header guard
