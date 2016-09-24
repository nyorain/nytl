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

#pragma once

#include <cstdint>

#include <nytl/fwd/flags.hpp>
#include <nytl/fwd/simplex.hpp>
#include <nytl/fwd/typemap.hpp>
#include <nytl/fwd/vec.hpp>
#include <nytl/fwd/rect.hpp>
#include <nytl/fwd/mat.hpp>

namespace nytl
{

class Logger;
class StringParam;
class Timer;
class StringParam;
class SizedStringParam;
class Observable;
class Observer;

template<typename ID> class Connectable;
template<typename ID> class Connection;
template<typename ID> class ConnectionRef;
template<typename ID> class ConnectionGuard;

template<typename Signature> class Callback;
using CbConn = Connection<std::size_t>;
using CbConnRef = ConnectionRef<std::size_t>;
using CbConnGuard = ConnectionGuard<std::size_t>;

template<typename T> class Range;
template<typename Signature> class CompatibleFunction;

template<typename T> class IntrusivePtr;
template<typename T> class ObservingPtr;

}
