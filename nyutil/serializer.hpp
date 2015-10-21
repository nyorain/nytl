/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Jan Kelling 
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

#include <nyutil/typemap.hpp>
#include <string>

namespace nyutil
{

//typeName
template<typename T> struct typeName
	{ static std::string name(){ return T::typeName(); }; };
template<typename T> struct typeName<const T>
	{ static std::string name(){ return "const" + typeName<T>(); } };
template<typename T> struct typeName<T*>
	{ static std::string name(){ return typeName<T>() + "*"; } };
template<typename T> struct typeName<T&>
	{ static std::string name(){ return typeName<T>() + "&"; } };
template<typename T> struct typeName<T* const>
	{ static std::string name(){ return typeName<T>() + "* const"; } };

#define REG_TYPE_NAME(Type) \
	namespace nyutil { \
   	template<> \
	struct typeName<Type> \
	{ \
		static std::string name(){ return #Type; }; \
   	}; \
	}

#define REG_TEMPLATE_TYPE_NAME(Type) \
	namespace nyutil { \
	template<typename... P> \
	struct typeName<Type<P>> \
	 \
		static std::string name() \
		{ \
			std::string ret(#Type); \
			int dummy[] = {((void) ret.append(typeName<P>()) , 0)...}; \
			return ret; \
		}; \
	}; \
	}

//serialzer
template<typename Base>
class serializer : public typemap<std::string, Base>
{
public:
	template<typename T>
	std::size_t registerType(const std::string& id = typeName<T>::name())
	{	
		return typemap<std::string, Base>::template registerType<T>(id);
	};	
};

}

REG_TYPE_NAME(int);
REG_TYPE_NAME(unsigned int);
REG_TYPE_NAME(char);
REG_TYPE_NAME(unsigned char);
REG_TYPE_NAME(long);
REG_TYPE_NAME(unsigned long);
REG_TYPE_NAME(short);
REG_TYPE_NAME(unsigned short);
REG_TYPE_NAME(float);
REG_TYPE_NAME(double);
REG_TYPE_NAME(std::string);


