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
///An example (and implemented by nytl) is Callback.
template<typename ID>
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
template <typename ID>
class BasicConnectionGuard
{
public:
	BasicConnectionGuard() noexcept = default;
	BasicConnectionGuard(BasicConnectable<ID>& conn, ID id) : conn_(&conn), id_(id) {}
	virtual ~BasicConnectionGuard() { disconnect(); }

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
	void connected() const { return (conn_); }
	ID release() { auto cpy = id_; id_ = {}; conn_ = {}; return cpy; }

	BasicConnectable<ID>& connectable() const { return *conn_; }
	ID id() const { return id_; }

protected:
	BasicConnectable<ID>* conn_ {};
	ID id_ {};
};

///Associates a BasicConnectable implementation with one of its connection ids.
///Note that this does not automatically destroy the connection, nor does is 
///track the lifetime of the BasicConnectable implementation object.
///Destroying the associated Connectable object during the lifetime
///of the Connection object without then explicitly releasing the Connection id results
///in undefined behaviour.
///Same as BasicConnectionRef, but does not destroy the connection it holds on destruction.
template <typename ID>
class BasicConnection
{
public:
	BasicConnection() noexcept = default;
	BasicConnection(BasicConnectable<ID>& conn, ID id) : conn_(&conn), id_(id) {}
	~BasicConnection() = default;

	BasicConnection(const BasicConnection& lhs) noexcept = default;
	BasicConnection& operator=(const BasicConnection& lhs) noexcept = default;

	void disconnect() { if(conn_) conn_->disconnect(id_); conn_ = {}; id_ = {}; }
	void connected() const { return (conn_); }

	BasicConnectable<ID>& connectable() const { return *conn_; }
	ID id() const { return id_; }

protected:
	BasicConnectable<ID>* conn_ {};
	ID id_ {};
};

template<typename ID>
auto makeConnection(BasicConnectable<ID>& conn, ID id) { return BasicConnection<ID>(conn, id); }

using ConnectionID = struct ConnectionIDType*;
using Connectable = BasicConnectable<ConnectionID>;
using Connection = BasicConnection<ConnectionID>;
using ConnectionGuard = BasicConnectionGuard<ConnectionID>;

}

#endif //header guard
