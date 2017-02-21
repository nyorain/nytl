// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the Connection and Connectable classes needed e.g. for callbacks.

#pragma once

#ifndef NYTL_INCLUDE_CONNECTION_HPP
#define NYTL_INCLUDE_CONNECTION_HPP

#include <iostream> // std::cerr
#include <exception> // std::exception
#include <memory> // std::shared_ptr

namespace nytl {

/// \brief Interface for classes that can be connected to in some way.
/// An example (in nytl) is nytl::Callback.
/// The way for obtaining such a connection if class-defined, this interface defines
/// only a common way to disconnect again, which can then be used by the connection classes.
/// Using this abstraction makes e.g. the Connection class possible as generic
/// connection, seperated from the type of the class it has a connection for.
template <typename ID>
class BasicConnectable {
public:
	virtual ~BasicConnectable() = default;
	virtual bool disconnect(const ID& id) = 0;
};

/// \brief Associates a BasicConnectable implementation with one of its connection ids.
/// Note that this does not automatically destroy the connection, nor does is
/// track the lifetime of the BasicConnectable implementation object.
template <typename C, typename ID>
class BasicConnection {
public:
	BasicConnection() noexcept = default;
	BasicConnection(C& conn, ID id) : conn_(&conn), id_(id) {}
	~BasicConnection() = default;

	BasicConnection(const BasicConnection& lhs) noexcept = default;
	BasicConnection& operator=(const BasicConnection& lhs) noexcept = default;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const { return conn_ && id_.valid(); }

	C* connectable() const { return conn_; }
	ID id() const { return id_; }

protected:
	C* conn_ {};
	ID id_ {};
};

/// \brief RAII wrapper around a connection id.
/// Note that this does not observe the lifetime of the object the connection id
/// was received from. Destroying the associated Connectable object during the lifetime
/// of the Connection object without then explicitly releasing the Connection id results
/// in undefined behaviour. Same as BasicConnection, but owns the connection
/// it holds, i.e. disconnects it on destruction. Therefore there should never be multiple
/// BasicConnectionGuards for the same connection id. If there exists a connection guard
/// for a connection this connection should not be disconnected in any other way than
/// the destruction of the guard (except the guard is explicitly released).
/// \reqruies Type 'C' shall be disconnectable, i.e. implement disconnect() member function.
/// \reqruies Type 'ID' shall be default and copy constructable/assignable.
template<typename C, typename ID>
class BasicUniqueConnection {
public:
	BasicUniqueConnection() = default;
	BasicUniqueConnection(C& conn, ID id) : conn_(&conn), id_(id) {}
	BasicUniqueConnection(BasicConnection<C, ID> lhs) : conn_(lhs.connectable()), id_(lhs.id()) {}
	~BasicUniqueConnection();

	BasicUniqueConnection(BasicUniqueConnection&& lhs) noexcept;
	BasicUniqueConnection& operator=(BasicUniqueConnection&& lhs) noexcept;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const noexcept { return conn_ && id_.valid(); }

	/// Releases ownership of the associated connection and returns its id.
	/// After the call this object will be empty.
	ID release() { auto cpy = id_; id_ = {}; conn_ = {}; return cpy; }

	C* connectable() const { return conn_; }
	ID id() const { return id_; }

protected:
	C* conn_ {};
	ID id_ {};
};

// using ConnectionID = struct ConnectionIDType*;
struct ConnectionID {
	std::size_t value;

	constexpr void reset() {}
	constexpr bool valid() const { return value; }
};

constexpr bool operator==(ConnectionID a, ConnectionID b) { return a.value == b.value; }
constexpr bool operator!=(ConnectionID a, ConnectionID b) { return a.value != b.value; }

struct TrackedConnectionID {
	std::shared_ptr<std::size_t> value;

	TrackedConnectionID() = default;
	TrackedConnectionID(std::size_t val) : value(std::make_shared<std::size_t>(val)) {}

	void reset() { if(value) *value = 0; value.reset(); }
	bool valid() const { return value && *value; }
};

bool operator==(const TrackedConnectionID& a, const TrackedConnectionID& b)
{
	return a.value == b.value;
}
bool operator!=(const TrackedConnectionID& a, const TrackedConnectionID& b)
{
	return a.value != b.value;
}

using Connectable = BasicConnectable<ConnectionID>;
using Connection = BasicConnection<Connectable, ConnectionID>;
using UniqueConnection = BasicUniqueConnection<Connectable, ConnectionID>;

using TrackedConnectable = BasicConnectable<TrackedConnectionID>;
using TrackedConnection = BasicConnection<TrackedConnectable, TrackedConnectionID>;
using TrackedUniqueConnection = BasicUniqueConnection<TrackedConnectable, TrackedConnectionID>;

// - helper functions to create connection objects -
// only needed until C++!7
template<typename C, typename ID>
auto makeConnection(C& connectable, ID id)
	{ return BasicConnection<ID, C>(connectable, id); }

template<typename C, typename ID>
auto makeUniqueConnection(C& connectable, ID id)
	{ return BasicUniqueConnection<ID, C>(connectable, id); }

// - implementation -
template<typename C, typename ID> BasicUniqueConnection<C, ID>&
BasicUniqueConnection<C, ID>::operator=(BasicUniqueConnection&& lhs) noexcept
{
	try {
		disconnect();
	} catch(const std::exception& error) {
		std::cerr << "nytl::BasicUniqueConnection: disconnect failed: " << error.what() << "\n";
	}

	conn_ = lhs.conn_;
	id_ = lhs.id_;
	lhs.conn_ = {};
	lhs.id_ = {};
	return *this;
}

template<typename C, typename ID>
BasicUniqueConnection<C, ID>::~BasicUniqueConnection()
{
	try {
		disconnect();
	} catch(const std::exception& error) {
		std::cerr << "nytl::~BasicUniqueConnection: disconnect failed: " << error.what() << "\n";
	}
}

template<typename C, typename ID>
BasicUniqueConnection<C, ID>::BasicUniqueConnection(BasicUniqueConnection&& lhs) noexcept
	: conn_(lhs.conn_), id_(lhs.id_)
{
	lhs.id_ = {};
	lhs.conn_ = {};
}

} // namespace nytl

#endif // header guard
