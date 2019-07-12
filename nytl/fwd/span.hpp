// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_SPAN
#define NYTL_INCLUDE_FWD_SPAN

#include <cstdlib> // std::size_t

namespace nytl {
	constexpr const std::size_t dynamic_extent = -1;
	template<typename T, std::size_t N = dynamic_extent> class span;
	template<typename T, std::size_t N = dynamic_extent>
	using Span = span<T, N>;
}

#ifdef NYTL_SPAN_STD

	// undefined behavior
	// span will be removed here with c++20 anyways
	namespace std {
		using nytl::span;
	}

#endif // NYTL_SPAN_STD
#endif // header guard
