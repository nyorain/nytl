// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SCOPE
#define NYTL_INCLUDE_SCOPE

#include <exception> // std::uncaught_exceptions
#include <iostream> // std::cerr

namespace nytl {

/// \brief Can be used to execute code when going out of the current scope.
/// This enables to handle non-RAII resources in a pseudo-RAII manner without
/// having to define own wrapper functions.
/// Example for how this could be useful for e.g. file descriptors.
/// Note that no matter which way we take out of the shown scope
/// (exception, early return or just coming to the scopes end) the given scope
/// guard will be executed and the fd closed.
/// All exceptions will always be caught and outputted to cerr.
/// ```cpp
/// {
/// 	auto fd = ::open("test.txt");
/// 	auto fdGuard = nytl::ScopeGuard([=]{ ::close(fd); }};
///
///		if(condition()) return;
///		functionThatMightThrow();
/// }
/// ```
template<typename F, bool OnSuccess = true, bool OnException = true>
class ScopeGuard {
public:
	static_assert(OnSuccess || OnException);

public:
	ScopeGuard(F&& func) :
		func_(std::forward<F>(func)),
		exceptions_(std::uncaught_exceptions()) {}

	ScopeGuard(ScopeGuard&&) = delete;
	ScopeGuard& operator =(ScopeGuard&&) = delete;

	~ScopeGuard() noexcept {
		if(exceptions_ == -1) {
			return;
		}

		try {
			auto thrown = exceptions_ < std::uncaught_exceptions();
			if((OnSuccess && !thrown) || (OnException && thrown)) {
				func_();
			}
		} catch(const std::exception& err) {
			std::cerr << "~nytl::ScopeGuard: exception while unwinding: ";
			std::cerr << err.what() << std::endl;
		} catch(...) {
			std::cerr << "~nytl::ScopeGuard: caught non-exception while ";
			std::cerr << "unwinding" << std::endl;
		}
	}

	void unset() { exceptions_ = -1; }

protected:
	F func_;
	int exceptions_;
};

/// Utility shortcuts to only execute a function when the scope is left
/// with/without exceptions. See ScopeGuard for details.
template<typename F>
class SuccessGuard : public ScopeGuard<F, true, false> {
public:
	using ScopeGuard<F, true, false>::ScopeGuard;
};

template<typename F>
class ExceptionGuard : public ScopeGuard<F, false, true> {
public:
	using ScopeGuard<F, false, true>::ScopeGuard;
};


// While the deduction guides are only needed for the derived classes,
// they also make sure that ScopeGuard can be constructed with
// an lvalue, which will result in storing the passed function by reference:
// ```cpp
// std::function f = []{ std::cout << "foo\n"; };
// ScopeGuard fooOrBar(f); // this captures by reference
// f = []{ std::cout << "bar\n"; };
// // the ScopeGuard will output bar when leaving the scope
// ```
template<typename F> ScopeGuard(F&&) -> ScopeGuard<F>;
template<typename F> SuccessGuard(F&&) -> SuccessGuard<F>;
template<typename F> ExceptionGuard(F&&) -> ExceptionGuard<F>;

} // namespace nytl

#endif // header guard
