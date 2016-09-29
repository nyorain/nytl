// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///Utility templates for associating a type with a name

#pragma once

#include <nytl/bits/templateString.inl>
#include <nytl/bits/tmpUtil.inl>
#include <nytl/bits/typeNameFunc.inl>
#include <string>

namespace nytl
{

///\ingroup utility
template<typename Name>
class Named;

template<char... Name>
class Named<TemplateString<Name...>>
{
public:
	using NameString = TemplateString<Name...>;
	using NamedType = Named;

	static constexpr ConstString typeName() { return NameString::constString(); }
};

///\ingroup utility
template<typename Base, typename Name>
class DeriveNamed;

template<typename Base, char... Name>
class DeriveNamed<Base, TemplateString<Name...>> : public Base
{
public:
	using NameString = TemplateString<Name...>;
	using NamedType = DeriveNamed;

	static constexpr ConstString typeName() { return NameString::constString(); }
};


//prototypes
///\ingroup utility
template<typename T, typename = void> struct TypeName;

///\ingroup utility
template<typename... T> struct TypeNames;

//convinience functions
///\ingroup utility
template<typename T>
std::string typeName(bool space = 0)
	{ return TypeName<T>::name(space); }

///\ingroup utility
template<typename... T>
std::string typeNames(bool space = 0)
	{ return TypeNames<T...>::names(space); }

//specialized structs
template<typename T, typename>
struct TypeName
	{ static std::string name(bool){ return detail::typeNameFunc<T>(); } };

template<typename T>
struct TypeName<T, void_t<decltype(T::typeName())>>
	{ static std::string name(bool space){ return T::typeName(space); } };

template<typename T>
struct TypeName<const T>
	{ static std::string name(bool space){ return "const " + typeName<T>(space); } };

template<typename T>
struct TypeName<T*>
	{ static std::string name(bool){ return typeName<T>() + "*"; } };

template<typename T>
struct TypeName<T&>
	{ static std::string name(bool){ return typeName<T>() + "&"; } };

template<typename T>
struct TypeName<T* const>
	{ static std::string name(bool){ return typeName<T>() + "* const"; } };

template<typename Ret, typename... Args>
struct TypeName<Ret(Args...)>
	{ static std::string name(bool)
		{ return typeName<Ret>() + "("  + typeNames<Args...>() + ")"; }
	};

//multiple
template<typename... T>
struct TypeNames
{
	static std::string names(bool space)
	{
		std::string ret {};
		bool first = 1;
		auto fnc = [&](const std::string& s)
			{
				if(!first) ret.append(", ");
				else first = 0;
				ret.append(s);
			};

		using expander = int[];
		int counter = 0;
		expander{((void) fnc(typeName<T>((++counter == sizeof...(T) && space) ? 1 : 0)) , 0)...};
		return ret;
	}
};


}

#define NYTL_GEN_TYPENAME(Type) \
	namespace nytl { \
	template<> struct TypeName<Type> \
		{ static std::string name(bool){ return #Type; }; }; \
	}

#define NYTL_GEN_TEMPLATE_TYPENAME(Type) \
	namespace nytl { \
	template<typename... P> \
	struct TypeName<Type<P...>> \
	{ \
		static std::string name(bool space) \
			{ return std::string(#Type) + "<" + typeNames<P...>(1) + \
				(space ? "> " : ">"); }; \
	}; \
	}
