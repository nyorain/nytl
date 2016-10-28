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
	virtual bool removeConnection(ID id) = 0;
};

///RAII wrapper around a connection id.
///Note that this does not observe the lifetime of the object the connection id
///was received from. Destroying the associated Connectable object during the lifetime
///of the Connection object without then explicitly releasing the Connection id results
///in undefined behaviour.
template <typename ID>
class BasicConnection
{
public:
	BasicConnection() noexcept = default;
	BasicConnection(BasicConnectable<ID>& conn, ID id) : conn_(&conn), id_(id) {}
	virtual ~BasicConnection() { destroy(); }

	BasicConnection(BasicConnection&& lhs) noexcept : conn_(lhs.conn_), id_(std::move(lhs.id_))
	{
		lhs.id_ = {};
		lhs.conn_ = {};
	}

	BasicConnection& operator=(BasicConnection&& lhs) noexcept
	{
		destroy();
		conn_ = lhs.conn_;
		id_ = std::move(lhs.id_);
		lhs.conn_ = {};
		lhs.id_ = {};
		return *this;
	}

	void destroy() { if(conn_) conn_->removeConnection(id_); conn_ = nullptr; id_ = {}; }
	void valid() const { return (conn_); }
	ID release() { auto cpy = id_; id_ = {}; return cpy; }

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

}

#endif //header guard
