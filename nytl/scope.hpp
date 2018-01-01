// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SCOPE
#define NYTL_INCLUDE_SCOPE

#include <exception> // std::uncaught_exceptions

namespace nytl {

// TODO: try/catch guards for called functions?
// In the case of ExceptionGuard they would result in std::terminate 100%

/// \brief Can be used to execute code when going out of the current scope.
/// This enables to handle non-RAII resources in a pseudo-RAII manner without having to
/// define own wrapper functions.
/// Example for how this could be useful for e.g. file descriptors. Note that no matter
/// which way we take out of the shown scope (exception, early return or just coming to the
/// scopes end) the given scope guard will be executed and the fd closed.
/// ```cpp
/// {
/// 	auto fd = ::open("test.txt");
/// 	auto fdGuard = nytl::ScopeGuard([=]{ ::close(fd); }};
///
///		if(condition()) return;
///		functionThatMightThrow();
/// }
/// ```
/// Alternatively, nytl defines the [NYTL_SCOPE_EXIT]() macro that can be used to easier
/// define scope guards.
/// \module utility
template<typename F>
class ScopeGuard {
public:
	ScopeGuard(F&& func) : func_(std::forward<F>(func)) {}
	~ScopeGuard() { func_(); }

protected:
	F func_;
};

/// \brief Can be used to execute code only when going out of scope due to a normal return.
/// \module utility
template<typename F>
class SuccessGuard {
public:
	SuccessGuard(F&& func)
		: func_(std::forward<F>(func)), exceptions_(std::uncaught_exception()) {}
	~SuccessGuard() { if(exceptions_ <= std::uncaught_exceptions()) func_(); }

protected:
	F func_;
	int exceptions_;
};

/// \brief Can be used to execute code only when going out of scope due to an exception.
/// \module utility
template<typename F>
class ExceptionGuard {
public:
	ExceptionGuard(F&& func)
		: func_(std::forward<F>(func)), exceptions_(std::uncaught_exception()) {}
	~ExceptionGuard() { if(exceptions_ > std::uncaught_exceptions()) func_(); }

protected:
	F func_;
	int exceptions_;
};

} // namespace nytl

#endif // header guard

/// \file Utilities for handling scope lifetimes, i.e. executing a function at the end of scope.
/// Examples for the different scope guards:
/// ```cpp
/// auto fdGuard = nytl::ScopeGuard([=]{ ::close(fd); });
/// auto successGuard = nytl::SuccessGuard([=]{ std::cout << "success!\n"; });
/// auto exceptionGuard = nytl::ExceptionGuard([=]{ std::cout << "exception!\n"; });
///
/// // the same as above but used with the nytl macros
/// NYTL_SCOPE_EXIT([=]{ ::close(fd); });
/// NYTL_SCOPE_SUCCESS([=]{ std::cout << "scope exited succesful!\n"; });
/// NYTL_SCOPE_EXCEPTION([=]{ std::cout << "scope exited due to an exception!\n"; });
/// ```
