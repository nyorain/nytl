// Copyright (c) 2017-2019 nyorain
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
#include <nytl/fwd/span.hpp>
#include <nytl/fwd/vec.hpp>
#include <nytl/fwd/mat.hpp>

namespace nytl {

// connection.hpp
template <typename ID> class ConnectableT;
template <typename C, typename ID> class ConnectionT;
template <typename C, typename ID> class UniqueConnectionT;

struct ConnectionID;
struct TrackedConnectionID;

using Connectable = ConnectableT<ConnectionID>;
using Connection = ConnectionT<Connectable, ConnectionID>;
using UniqueConnection = UniqueConnectionT<Connectable, ConnectionID>;

using TrackedConnectable = ConnectableT<TrackedConnectionID>;
using TrackedConnection = ConnectionT<TrackedConnectable, TrackedConnectionID>;
using TrackedUniqueConnection = UniqueConnectionT<TrackedConnectable, TrackedConnectionID>;

} // namespace nytl

#endif // header guad
