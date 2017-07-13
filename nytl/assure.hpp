// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

// NOTE: this file should not have a header guard
// Files that include it should undefine the macro at the end of their file
// It can be included again.

#include <string> // std::string

/// Define this macro to 0 before including any nytl headers to disable
/// any runtime checks. May lead to segmentation faults when invalid parameters are
/// given.
#ifndef NYTL_RUNTIME_CHECK
	#define NYTL_RUNTIME_CHECK 1
#endif

/// Utility macro that makes sure expr is fulfilled.
/// Will test it at compile time if static_ is true.
#define nytl_assure(static_, expr, message) \
	if constexpr(static_) { \
		static_assert(expr, message); \
	} else if constexpr(NYTL_RUNTIME_CHECK) { \
		if(!(expr)) {\
			std::string msg = __FUNCTION__; \
			msg += ": " message; \
			throw std::invalid_argument(msg); \
		} \
	}
