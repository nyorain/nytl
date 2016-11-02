// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines the Connection and Connectable classes needed e.g. for callbacks.

#pragma once

#ifndef NYTL_INCLUDE_CONNECTION_HPP
#define NYTL_INCLUDE_CONNECTION_HPP

#include <nytl/nonCopyable.hpp>

#include <cstdlib>
#include <memory>
#include <utility>

namespace nytl
{

///Interface for classes that can be connected to in some way.
///An example (in nytl) is nytl::Callback.
///The way for obtaining such a connection if implementation-defined, this interface defines
///only a common way to disconnect again. 
///Using this abstraction makes e.g. the Connection and Connection class possible for the
///cost of making the class virtual.
template <typename ID>
class BasicConnectable
{
public:
	virtual ~BasicConnectable() = default;
	virtual bool disconnect(ID id) = 0;
};

///RAII wrapper around a connection id.
///Note that this does not observe the lifetime of the object the connection id
///was received from. Destroying the associated Connectable object during the lifetime
///of the Connection object without then explicitly releasing the Connection id results
///in undefined behaviour. Same as BasicConnection, but owns the connection
///it holds, i.e. disconnects it on destruction.
template <typename Base, typename ID>
class BasicConnectionGuard
{
public:
	BasicConnectionGuard() noexcept = default;
	BasicConnectionGuard(Base& conn, ID id) : conn_(&conn), id_(id) {}
	BasicConnectionGuard(BasicConnection<Base, ID> lhs) : conn_(lhs.connectable()), id_(lhs.id()) {}
	~BasicConnectionGuard() { disconnect(); }

	BasicConnectionGuard(BasicConnectionGuard&& lhs) noexcept 
		: conn_(lhs.conn_), id_(std::move(lhs.id_))
	{
		lhs.id_ = {};
		lhs.conn_ = {};
	}

	BasicConnectionGuard& operator=(BasicConnectionGuard&& lhs) noexcept
	{
		disconnect();
		conn_ = lhs.conn_;
		id_ = std::move(lhs.id_);
		lhs.conn_ = {};
		lhs.id_ = {};
		return *this;
	}

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const { return (conn_); }
	ID release() { auto cpy = id_; id_ = {}; conn_ = {}; return cpy; }

	Base* connectable() const { return *conn_; }
	ID id() const { return id_; }

protected:
	Base* conn_ {};
	ID id_ {};
};

///Associates a BasicConnectable implementation with one of its connection ids.
///Note that this does not automatically destroy the connection, nor does is 
///track the lifetime of the BasicConnectable implementation object.
///Destroying the associated Connectable object during the lifetime
///of the Connection object without then explicitly releasing the Connection id results
///in undefined behaviour.
///Same as BasicConnectionGuard, but does not destroy the connection it holds on destruction.
template <typename Base, typename ID>
class BasicConnection
{
public:
	BasicConnection() noexcept = default;
	BasicConnection(Base& conn, ID id) : conn_(&conn), id_(id) {}
	~BasicConnection() = default;

	BasicConnection(const BasicConnection& lhs) noexcept = default;
	BasicConnection& operator=(const BasicConnection& lhs) noexcept = default;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	bool connected() const { return (conn_); }

	Base* connectable() const { return *conn_; }
	ID id() const { return id_; }

protected:
	Base* conn_ {};
	ID id_ {};
};

///Helper function to create connections
template <typename ID, typename Base>
auto makeConnection(Base& base, ID id) 
	{ return BasicConnection<ID, Base>(base, id); }

template <typename ID, typename Base>
auto makeConnectionGuard(Base& base, ID id) 
	{ return BasicConnectionGuard<ID, Base>(base, id); }

using ConnectionID = struct ConnectionIDType*;
using Connectable = BasicConnectable<ConnectionID>;
using Connection = BasicConnection<Connectable, ConnectionID>;
using ConnectionGuard = BasicConnectionGuard<Connectable, ConnectionID>;

}

#endif //header guard
