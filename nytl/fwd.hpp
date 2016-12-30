// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Contains forward declarations for the major ny classes and templates.
/// This file only contains fwd declarations for classes where they might be needed, not
/// for pure meta templates. Classes with special forward declarations (such as multiple
/// typedefs or default template parameters) are excluded to special files in nytl/fwd/.

#pragma once

#ifndef NYTL_INCLUDE_FWD
#define NYTL_INCLUDE_FWD

#include <nytl/fwd/flags.hpp>
#include <nytl/fwd/simplex.hpp>
#include <nytl/fwd/typemap.hpp>
#include <nytl/fwd/referenced.hpp>
#include <nytl/fwd/span.hpp>
#include <nytl/fwd/vec.hpp>
#include <nytl/fwd/mat.hpp>

namespace nytl {

// stringParam.hpp
class StringParam;
class SizedStringParam;

// observe.hpp
template<typename T> class Observable;
template<typename T> class Observer;
template<typename T> class ObservingPtr;

// connection.hpp
template<typename ID> class BasicConnectable;
template<typename Base, typename ID> class BasicConnection;
template<typename Base, typename ID> class BasicConnectionGuard;
template<typename Base, typename ID> class BasicConnectionRef;

using ConnectionID = struct ConnectionIDType*;
using Connectable = BasicConnectable<ConnectionID>;

using Connection = BasicConnection<Connectable, ConnectionID>;
using ConnectionRef = BasicConnectionRef<Connectable, ConnectionID>;
using ConnectionGuard = BasicConnectionGuard<Connectable, ConnectionID>;

// callback.hpp
template<typename Signature> class Callback;

} // namespace nytl

#endif // header guad
