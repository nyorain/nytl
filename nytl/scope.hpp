// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///Defines ScopeGuard that can be used to execute a function when going out of scope.

//TODO: possiblity to differenciate between "succeeded" and "failed (i.e. exception)" scope
//we cannot simply use std::uncaught_exception since if the scope in which the ScopeGuard
//lives is itself called during stack unwinding (i.e. from a destructor) the scope would
//always "fail"
//
//if one wants to make a destructor throw, simply makde a destroy/close function that might
//throw and then call it in the destructor using a try/catch block.
//C++17 will allow us determine the ScopeExitReason.

#pragma once

namespace nytl
{

///Utility template that can be used to execute code when going out of scope.
template<typename F>
class ScopeGuard
{
public:
	F function;

public:
	ScopeGuard(const F& f) : function(f) {}
	~ScopeGuard() { function(); }
};

template<typename F> ScopeGuard<F> makeScopeGuard(const F& func) { return {func}; }

#define CAT_IMPL(A, B) A ## B
#define CAT(A, B) CAT_IMPL(A, B)

#define NYTL_SCOPE_EXIT(x) auto CAT(nytlScopeGuard, __LINE__) = makeScopeGuard(x);

#undef CAT
#undef CAT_IMPL

}
