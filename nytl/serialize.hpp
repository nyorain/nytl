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

#include <nytl/typemap.hpp>
#include <nytl/log.hpp>

#include <string>
#include <type_traits>
#include <fstream>

namespace nytl
{

//typeNameFallback
template<typename A> struct ignored
{
    using type = void;
};

//
template<typename... Args> struct typeNames;

//typeName
template<typename T, typename Cond = void> struct typeName
	{ static std::string name(){ return typeid(T{}).name(); }; };
template<typename T> struct typeName<T, typename ignored<decltype(T::typeName())>::type>
	{ static std::string name(){ return T::typeName(); }; };

template<typename T> struct typeName<const T>
	{ static std::string name(){ return "const" + typeName<T>(); } };
template<typename T> struct typeName<T*>
	{ static std::string name(){ return typeName<T>() + "*"; } };
template<typename T> struct typeName<T&>
	{ static std::string name(){ return typeName<T>() + "&"; } };
template<typename T> struct typeName<T* const>
	{ static std::string name(){ return typeName<T>() + "* const"; } };
template<typename Ret, typename... Args> struct typeName<Ret(Args...)>
	{ static std::string name(){ return typeName<Ret>() + "(" + typeNames<Args...>() + ")"; } };

template<typename... Args> struct typeNames
{
    static std::string name()
    {
        std::string ret {};
        bool first = 1;
        auto fnc = [&](const std::string& s)
            {
                if(!first) ret.append(",");
                else first = 0;
                ret.append(s);
            };

        using expander = int[];
        expander{((void) fnc(typeName<Args>()) , 0)...};
        return ret;
    }
};


#define NYTL_REG_TYPE_NAME(Type) \
	namespace nytl { \
   	template<> \
	struct typeName<Type> \
	{ \
		static std::string name(){ return #Type; }; \
   	}; \
	}

#define NYTL_REG_TEMPLATE_TYPE_NAME(Type) \
	namespace nytl { \
	template<typename... P> \
	struct typeName<Type<P...>> \
	 \
		static std::string name() \
		{ \
			std::string ret(#Type); \
			int[]{((void) ret.append(typeName<P>()) , 0)...}; \
			return ret; \
		}; \
	}; \
	}

//serialzer
template<typename Base, typename... CArgs>
class serializer : public typemap<std::string, Base, CArgs...>
{
public:
	template<typename T>
	std::size_t registerType(const std::string& id = typeName<T>::name())
	{
		return typemap<std::string, Base, CArgs...>::template registerType<T>(id);
	};
};

//serialized
template<typename Base, typename... CArgs>
class serialized
{
public:
    static serializer<Base, CArgs...>& derivedTypes()
    {
        static serializer<Base, CArgs...> instance;
        return instance;
    };

	template<typename T>
	static std::size_t registerType()
	{
		return derivedTypes().template registerType<T>();
	}

	template<typename T>
	static std::size_t registerType(const std::string& str)
	{
		return derivedTypes().template registerType<T>(str);
	}

    //create
    static std::unique_ptr<Base> create(const std::string& name, CArgs... args)
    {
        return derivedTypes().createObject(name, args...);
    }

    static std::unique_ptr<Base> createLoad(std::istream& is, CArgs... args)
    {
        std::string name;
        std::getline(is, name);
        auto ptr = create(name, args...);
        if(ptr && ptr->load(is, args...)) return ptr;
        else return nullptr;
    }

	static std::unique_ptr<Base> createLoad(const std::string& filename, CArgs... args)
	{
		std::ifstream ifs(filename);
		if(!ifs.is_open())
		{
			sendWarning("serialized::createLoad: failed to open file ", filename);
			return nullptr;
		}

		auto ret = createLoad(ifs, args...);
		ifs.close();

		return ret;
	}

    //getName
    template<typename T> static std::string getName(const T& ref = T{})
    {
        auto it = derivedTypes().getByTypeInfo(typeid(ref));

        if(derivedTypes().valid(it)) return it->first;
        return std::string{};
    }

public:
    virtual bool load(std::istream& is, CArgs... args) { return 1; };
    virtual bool save(std::ostream& os) const { os << name() << "\n"; return 1; }

    bool saveFile(const std::string& filename) const;
    bool loadFile(const std::string& filename);

    virtual std::string name() const = 0;
};

//save
template<typename Base, typename... CArgs> bool
serialized<Base, CArgs...>::saveFile(const std::string& filename) const
{
    std::ofstream ofs(filename);
    if(!ofs.is_open())
    {
        sendWarning(name(), "::save: failed to open ", filename);
        return 0;
    }

	bool ret = save(ofs);
	ofs.close();

	return ret;
}

//load
template<typename Base, typename... CArgs> bool
serialized<Base, CArgs...>::loadFile(const std::string& filename)
{
    std::ifstream ifs(filename);
    if(!ifs.is_open())
    {
        std::cout << name() << "load: failed to open " << filename << "\n";
        return 0;
    }

    std::string tmp;
    std::getline(ifs, tmp);

	bool ret = load(ifs);
	ifs.close();

	return ret;
}


//nameFunc
#define NYTL_NAME_FUNC() virtual std::string name() const override { return this->getName(*this); }

//get
#define NYTL_SERIALIZE_GET_VERSION(a,b,c,d,Name,...) Name

//macro
#define NYTL_SERIALIZE_TYPE(...) \
    NYTL_SERIALIZE_GET_VERSION(__VA_ARGS__, NYTL_SERIALIZE_TYPE4, NYTL_SERIALIZE_TYPE3, NYTL_SERIALIZE_TYPE2)(__VA_ARGS__)

#define NYTL_SERIALIZE_TYPE2(Base, Derived)\
	NYTL_REG_TYPE_NAME(Derived);\
	namespace { \
	const bool serializeDummy_##Base_##Derived = Base::registerType<Derived>(); }

#define NYTL_SERIALIZE_TYPE3(Base, BaseName, Derived)\
	NYTL_REG_TYPE_NAME(Derived);\
	namespace { \
	const bool serializeDummy_##BaseName_##Derived = Base::registerType<Derived>(); }

#define NYTL_SERIALIZE_TYPE4(Base, BaseName, Derived, DerivedName) \
	NYTL_REG_TYPE_NAME(Derived); \
	namespace { \
	const bool serializeDummy_##BaseName_##DerivedName = Base::registerType<Derived>(); }

}

NYTL_REG_TYPE_NAME(std::int8_t);
NYTL_REG_TYPE_NAME(std::uint8_t);
NYTL_REG_TYPE_NAME(std::int16_t);
NYTL_REG_TYPE_NAME(std::uint16_t);
NYTL_REG_TYPE_NAME(std::int32_t);
NYTL_REG_TYPE_NAME(std::uint32_t);
NYTL_REG_TYPE_NAME(std::int64_t);
NYTL_REG_TYPE_NAME(std::uint64_t);
NYTL_REG_TYPE_NAME(float);
NYTL_REG_TYPE_NAME(double);
NYTL_REG_TYPE_NAME(void);
NYTL_REG_TYPE_NAME(std::string);


