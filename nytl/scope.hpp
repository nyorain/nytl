// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SCOPE
#define NYTL_INCLUDE_SCOPE

#include <exception> // std::uncaught_exceptions

namespace nytl {

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

// - utility macros for anonymous variable name -
// Since the NYTL_SCOPE_* macros create a variable, they have to give it a unique name
// and therefore use the CAT macro to join a prefix to the current line number.
#define CAT_IMPL(A, B) A ## B
#define CAT(A, B) CAT_IMPL(A, B)

/// \brief Utility macro for creating a scope guard.
/// `NYTL_SCOPE_EXIT(x)` is the same as `auto ... = nytl::ScopeGuard(x)` but reasonably
/// shorter and does not require a variable name.
/// \module utility
#define NYTL_SCOPE_EXIT(x) auto CAT(nytl_scopeGuard, __LINE__) = makeScopeGuard(x)

/// \brief Utility macro for creating a success scope guard.
/// `NYTL_SCOPE_SUCCESS(x)` is the same as `auto ... = nytl::SuccessGuard(x)` but reasonably
/// shorter and does not require a variable name.
/// \module utility
#define NYTL_SCOPE_SUCCESS(x) auto CAT(nytl_scopeGuard, __LINE__) = makeSuccessScopeGuard(x)

/// \brief Utility macro for creating an exception scope guard.
/// `NYTL_SCOPE_SUCCESS(x)` is the same as `auto ... = nytl::ExceptionGuard(x)` but reasonably
/// shorter and does not require a variable name.
/// \module utility
#define NYTL_SCOPE_EXCEPTION(x) auto CAT(nytl_scopeGuard, __LINE__) = makeExceptionScopeGuard(x)

// - undefine the utility macros -
#undef CAT
#undef CAT_IMPL

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
