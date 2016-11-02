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

template<typename ID> class BasicConnectable;
template<typename Base, typename ID> class BasicConnection;
template<typename Base, typename ID> class BasicConnectionGuard;
template<typename Base, typename ID> class BasicConnectionRef;

using ConnectionID = struct ConnectionIDType*;
using Connectable = BasicConnectable<ConnectionID>;

using Connection = BasicConnection<Connectable, ConnectionID>;
using ConnectionRef = BasicConnectionRef<Connectable, ConnectionID>;
using ConnectionGuard = BasicConnectionGuard<Connectable, ConnectionID>;

template<typename Signature> class Callback;
template<typename Signature> class CompatibleFunction;

template<typename T> class Range;
template<typename T> class IntrusivePtr;
template<typename T> class ObservingPtr;

}
