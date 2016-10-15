// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Contains forward declarations for the major ny classes and templates.

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
using CbIdType = unsigned int;
using CbConn = Connection<CbIdType>;
using CbConnRef = ConnectionRef<CbIdType>;
using CbConnGuard = ConnectionGuard<CbIdType>;

template<typename T> class Range;
template<typename Signature> class CompatibleFunction;

template<typename T> class IntrusivePtr;
template<typename T> class ObservingPtr;

}
