/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///Utility templates to name classes/types and serialize them.

#pragma once

#include <nytl/named.hpp>
#include <nytl/make_unique.hpp>
#include <nytl/typemap.hpp>
#include <nytl/cloneable.hpp>

#include <string>
#include <memory>
#include <ostream>
#include <istream>

namespace nytl
{

class SerializedBase : public AbstractCloneable<SerializedBase>
{
public:
	virtual std::string objectTypeName() const = 0;
	virtual const std::type_info& objectTypeInfo() const = 0;

	virtual bool load(std::istream&) { return 1; };
	virtual bool save(std::ostream& out) const
	{	
		out << objectTypeName() << '\n';
		return 1;
	};
};

template<typename T, typename Name>
class Serialized : public DeriveNamed<DeriveCloneable<SerializedBase, T>, Name>
{
public:
	using Base = SerializedBase;
	using typename DeriveNamed<DeriveCloneable<Base, T>, Name>::NamedBase;

	virtual std::string objectTypeName() const override { return NamedBase::typeName(); }
	virtual const std::type_info& objectTypeInfo() const override { return typeid(T); }
};

template<typename Base, typename Derived, typename Name>
class DeriveSerialized : public DeriveNamed<DeriveCloneable<Base, Derived>, Name>
{
public:
	using typename DeriveNamed<DeriveCloneable<Base, Derived>, Name>::NamedBase;

	virtual std::string objectTypeName() const override { return NamedBase::typeName(); }
	virtual const std::type_info& objectTypeInfo() const override { return typeid(Derived); }
};

template<typename Base = SerializedBase, typename... CArgs>
class Serializer : public Typemap<std::string, Base, CArgs...>
{
public:
	using TypemapBase = Typemap<std::string, Base, CArgs...>;
	using typename TypemapBase::Pointer;

public:
	Pointer createLoad(std::istream& in, CArgs... args) const 
	{
		std::string name;
		in >> name;
		auto ret = create(name, args...);
		if(!ret || !ret->load(in)) return nullptr;
		return ret;
	}

	using TypemapBase::add;

	template<typename T> std::size_t add(const T&) //easier to call
	{
		return add<T>(typeName<T>());
	}
	template<typename T> std::size_t add()
	{
		return add<T>(typeName<T>());
	}
};

}
