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

#pragma once

#include <nytl/bits/templateString.inl>
#include <nytl/bits/tmpUtil.inl>

namespace nytl
{

template<typename Base, typename Name>
class DeriveNamed;

template<typename Base, char... Name>
class DeriveNamed<Base, TemplateString<Name...>> : public Base
{
public:
	using NameString = TemplateString<Name...>;
	using NamedType = DeriveNamed;
	using NamedBase = DeriveNamed;

	static constexpr ConstString typeName() { return NameString::constString(); }
};

//structs
template<typename T>
struct TypeName : public DeriveDummy<decltype(T::typeName())>
	{ static constexpr auto value = T::typeName(); };

template<typename... T>
struct TypeNames
	{ static constexpr auto value = makeMultipleConstString(TypeName<T>::value...); };

//convinience functions
template<typename T>
constexpr auto typeName() -> decltype(TypeName<T>::value)
	{ return TypeName<T>::value; }

template<typename... T>
constexpr auto typeNames() -> decltype(TypeNames<T...>::value)
	{ return TypeNames<T...>::value; }

}

