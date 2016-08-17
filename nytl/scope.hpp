//
// The MIT License (MIT)
//
// Copyright (c) 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

///\file
///Defines ScopeGuard that can be used to execute a function when going out of scope.

//we cannot simply use std::uncaught_exception since if the scope in which the ScopeGuard
//lives is itself called during stack unwinding (i.e. from a destructor) the scope would
//always "fail"
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

//cat impl
#define CAT_IMPL(A, B) A ## B
#define CAT(A, B) CAT_IMPL(A, B)

#define NYTL_SCOPE_EXIT(x) auto CAT(nytlScopeGuard, __LINE__) = makeScopeGuard(x);

#undef CAT
#undef CAT_IMPL

}
