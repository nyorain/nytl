// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the Connection and Connectable classes needed e.g. for callbacks.

#pragma once

#ifndef NYTL_INCLUDE_CONNECTION
#define NYTL_INCLUDE_CONNECTION

#include <iostream> // std::cerr
#include <exception> // std::exception
#include <memory> // std::shared_ptr

namespace nytl {

/// \brief Interface for classes that can be connected to in some way.
/// An example (in nytl) is nytl::Callback.
/// The way for obtaining such a connection if class-defined, this interface defines
/// only a common way to disconnect again, which can then be used by the connection classes.
/// Using this abstraction makes e.g. the Connection class possible as generic
/// connection, separated from the type of the class it has a connection for.
template <typename ID>
class ConnectableT {
public:
	virtual ~ConnectableT() = default;
	virtual bool disconnect(const ID& id) = 0;
};

/// \brief Associates a BasicConnectable implementation with one of its connection ids.
/// Note that this does not automatically destroy the connection, nor does is
/// track the lifetime of the BasicConnectable implementation object.
template <typename C, typename ID>
class ConnectionT {
public:
	ConnectionT(C& connectable, ID id)
		: connectable_(&connectable), id_(id) {}

	ConnectionT() noexcept = default;
	~ConnectionT() = default;

	ConnectionT(const ConnectionT&) noexcept = default;
	ConnectionT& operator=(const ConnectionT&) noexcept = default;

	/// Disconnects the represented connection.
	void disconnect()
	{
		if(connectable_)
			connectable_->disconnect(id_);
		connectable_ = {};
		id_ = {};
	}

	/// Returns whether this connection object is valid.
	/// Note that depending on the connection id and callable type and if
	/// the callback was destroyed before the connection, this may
	/// not represent the status of the connection.
	bool connected() const noexcept { return connectable_ && id_.get() > 0; }

	/// The associated connectable object.
	C* connectable() const { return connectable_; }

	/// The associated connection id.
	ID id() const { return id_; }

protected:
	C* connectable_ {};
	ID id_ {};
};

/// \brief RAII wrapper around a connection id.
/// Note that this does not observe the lifetime of the object the connection id
/// was received from. Destroying the associated Connectable object during the lifetime
/// of the Connection object without then explicitly releasing the Connection id results
/// in undefined behavior. Same as BasicConnection, but owns the connection
/// it holds, i.e. disconnects it on destruction. Therefore there should never be multiple
/// BasicConnectionGuards for the same connection id. If there exists a connection guard
/// for a connection this connection should not be disconnected in any other way than
/// the destruction of the guard (except the guard is explicitly released).
/// \reqruies Type 'C' shall be Disconnectable, i.e. implement disconnect() member function, e.g
/// derived from ConnectableT<ID>
/// \reqruies Type 'ID' shall fulfill the ConntectableID concept.
template<typename C, typename ID>
class UniqueConnectionT {
public:
	UniqueConnectionT() = default;

	UniqueConnectionT(C& connectable, ID id)
		: connectable_(&connectable), id_(id) {}

	UniqueConnectionT(ConnectionT<C, ID> lhs)
		: connectable_(lhs.connectable()), id_(lhs.id()) {}

	~UniqueConnectionT()
	{
		try {
			disconnect();
		} catch(const std::exception& error) {
			std::cerr << "nytl::~UniqueConnectionT: disconnect failed: " << error.what() << "\n";
		}
	}

	UniqueConnectionT(UniqueConnectionT&& lhs) noexcept
		: connectable_(lhs.connectable_), id_(lhs.id_)
	{
		lhs.id_ = {};
		lhs.connectable_ = {};
	}

	UniqueConnectionT& operator=(UniqueConnectionT&& lhs) noexcept
	{
		try {
			disconnect();
		} catch(const std::exception& error) {
			std::cerr << "nytl::UniqueConnectionT: disconnect failed: " << error.what() << "\n";
		}

		connectable_ = lhs.connectable_;
		id_ = lhs.id_;
		lhs.connectable_ = {};
		lhs.id_ = {};
		return *this;
	}

	/// Disconnects the represented connection.
	void disconnect()
	{
		if(connected())
			connectable_->disconnect(id_);
		connectable_ = {};
		id_ = {};
	}

	/// The associated connectable object.
	C* connectable() const { return connectable_; }

	/// The associated connection id.
	ID id() const { return id_; }

	/// Returns whether this connection object is valid.
	/// Note that depending on the connection id and callable type and if
	/// the callback was destroyed before the connection, this may
	/// not represent the status of the connection.
	bool connected() const noexcept { return connectable_ && id_.get() > 0; }

	/// Releases ownership of the associated connection and returns a non-owned
	/// connection object.
	/// After the call this object will be empty.
	ID release()
	{
		ConnectionT<C, ID> ret{connectable_, id_};
		id_ = {};
		connectable_ = {};
		return ret;
	}

protected:
	C* connectable_ {};
	ID id_ {};
};

/// Default ID for a connection that is entirely defined over its value.
struct ConnectionID {
	std::int64_t value;

	void set(std::int64_t val) noexcept { value = val; }
	auto get() const noexcept { return value; }
	void removed() const noexcept {}
};

/// Shares the id value between all connections so that disconnections from
/// another connection or the callback itself can be observed.
struct TrackedConnectionID {
	std::shared_ptr<std::int64_t> value;

	TrackedConnectionID() = default;
	TrackedConnectionID(std::int64_t val) : value(std::make_shared<std::int64_t>(val)) {}

	void set(std::int64_t val) noexcept { if(value) *value = val; }
	auto get() const noexcept { return (value) ? *value : 0; }
	void removed() noexcept { if(value) *value = 0; value.reset(); }
};

using Connectable = ConnectableT<ConnectionID>;
using Connection = ConnectionT<Connectable, ConnectionID>;
using UniqueConnection = UniqueConnectionT<Connectable, ConnectionID>;

using TrackedConnectable = ConnectableT<TrackedConnectionID>;
using TrackedConnection = ConnectionT<TrackedConnectable, TrackedConnectionID>;
using TrackedUniqueConnection = UniqueConnectionT<TrackedConnectable, TrackedConnectionID>;

// TODO: remove
/*
constexpr inline bool operator==(ConnectionID a, ConnectionID b)
	{ return a.value == b.value; }
constexpr inline bool operator!=(ConnectionID a, ConnectionID b)
	{ return a.value != b.value; }
bool inline operator==(const TrackedConnectionID& a, const TrackedConnectionID& b)
	{ return a.value == b.value; }
bool inline operator!=(const TrackedConnectionID& a, const TrackedConnectionID& b)
	{ return a.value != b.value; }
*/

} // namespace nytl

#endif // header guard
