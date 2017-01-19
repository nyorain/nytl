// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines the Connection and Connectable classes needed e.g. for callbacks.

#pragma once

#ifndef NYTL_INCLUDE_CONNECTION_HPP
#define NYTL_INCLUDE_CONNECTION_HPP

namespace nytl {

/// \brief Interface for classes that can be connected to in some way.
/// An example (in nytl) is nytl::Callback.
/// The way for obtaining such a connection if class-defined, this interface defines
/// only a common way to disconnect again, which can then be used by the connection classes.
/// Using this abstraction makes e.g. the Connection and Connection class possible as generic
/// connections, seperated from the type of the class they have a connection for.
template <typename ID>
class BasicConnectable {
public:
	virtual ~BasicConnectable() = default;
	virtual bool disconnect(ID id) = 0;
};

/// \brief Associates a BasicConnectable implementation with one of its connection ids.
/// Note that this does not automatically destroy the connection, nor does is
/// track the lifetime of the BasicConnectable implementation object.
/// Same as BasicConnectionGuard, but does not destroy the connection it holds on destruction.
template <typename C, typename ID>
class BasicConnection {
public:
	BasicConnection() noexcept = default;
	BasicConnection(C& conn, ID id) : conn_(&conn), id_(id) {}
	~BasicConnection() = default;

	BasicConnection(const BasicConnection& lhs) noexcept = default;
	BasicConnection& operator=(const BasicConnection& lhs) noexcept = default;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const { return (conn_); }

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
/// BasicConnectionGuards for the same connection ids. If there exists a connection guard
/// for a connection this connection should not be disconnected in any other way than
/// the destruction of the guard (except the guard is explicitly released).
/// \reqruies Type 'C' shall be disconnectable, i.e. implement disconnect() member function.
/// \reqruies Type 'ID' shall be default and copy constructable/assignable.
template<typename C, typename ID>
class BasicConnectionGuard {
public:
	BasicConnectionGuard() noexcept = default;
	BasicConnectionGuard(C& conn, ID id) : conn_(&conn), id_(id) {}
	BasicConnectionGuard(BasicConnection<C, ID> lhs) : conn_(lhs.connectable()), id_(lhs.id()) {}
	~BasicConnectionGuard()
	{
		try {
			disconnect();
		} catch(...) TODO
	}

	BasicConnectionGuard(BasicConnectionGuard&& lhs) noexcept
		: conn_(lhs.conn_), id_(lhs.id_)
	{
		lhs.id_ = {};
		lhs.conn_ = {};
	}

	BasicConnectionGuard& operator=(BasicConnectionGuard&& lhs) noexcept
	{
		disconnect();
		conn_ = lhs.conn_;
		id_ = lhs.id_;
		lhs.conn_ = {};
		lhs.id_ = {};
		return *this;
	}

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const noexcept { return (conn_); }
	ID release() { auto cpy = id_; id_ = {}; conn_ = {}; return cpy; }

	C* connectable() const { return conn_; }
	ID id() const { return id_; }

protected:
	C* conn_ {};
	ID id_ {};
};

// - helper functions to create connection objects -
// only needed until C++!7
template<typename C, typename ID>
auto makeConnection(C& connectable, ID id)
	{ return BasicConnection<ID, C>(connectable, id); }

template<typename C, typename ID>
auto makeConnectionGuard(C& connectable, ID id)
	{ return BasicConnectionGuard<ID, C>(connectable, id); }

using ConnectionID = struct ConnectionIDType*;
using Connectable = BasicConnectable<ConnectionID>;
using Connection = BasicConnection<Connectable, ConnectionID>;
using ConnectionGuard = BasicConnectionGuard<Connectable, ConnectionID>;

} // namespace nytl

#endif // header guard
