// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <string> // std::string
#include <stdexcept> // std::runtime_error

/// Define this macro to 0 before including any nytl headers to disable
/// any runtime checks. May lead to segmentation faults when invalid parameters are
/// given.
#ifndef NYTL_RUNTIME_CHECK
	#define NYTL_RUNTIME_CHECK 1
#endif

#ifndef NYTL_INCLUDE_ASSURE
#define NYTL_INCLUDE_ASSURE

namespace nytl {

class AssureError : public std::runtime_error {
public:
	AssureError(const char* func, const char* msg)
		: runtime_error(""), func_(func), msg_(msg) {}

	const char* what() const noexcept override {
		static std::string string;
		string.clear();
		string += "nytl: ";
		string += func_;
		string += ": ";
		string += msg_;
		return string.c_str();
	}

	const char* func_;
	const char* msg_;
};

}

#endif // header guard


// NOTE: this part of the file should not have a header guard
// Files that include it should undefine the macro at the end of their file
// It can be included again.

// TODO: make this a macro?
// constexpr template<bool Static> assure(bool, const char);

/// Utility macro that makes sure expr is fulfilled.
/// Will test it at compile time if static_ is true.
#define nytl_assure(static_, sexpr, dexpr, message) \
	if constexpr(static_) { \
		static_assert(sexpr, message); \
	} else if constexpr(NYTL_RUNTIME_CHECK) { \
		if(!(dexpr)) {\
			throw nytl::AssureError(__FUNCTION__, message "(" #dexpr ")"); \
		} \
	}
