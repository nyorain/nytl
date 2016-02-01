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

#include <nytl/make_unique.hpp>

#include <unordered_map>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include <type_traits>

namespace nytl
{

namespace detail
{

template<typename Base, typename T, typename... Args> struct make_unique_wrapper
{
    static std::unique_ptr<Base> call(Args... args) { return make_unique<T>(args...); };
};

template<typename T, typename... Args> struct make_unique_wrapper<void, T, Args...>
{
    static void* call(Args... args) { return new T(args...); };
};

}


//typemap
template<typename Identifier, typename Base = void, typename... CArgs>
class typemap
{
public:
    using ptr_type = typename std::conditional<std::is_same<Base, void>::value, void*, std::unique_ptr<Base>>::type;

    //base
    struct typeBase
    {
    public:
        virtual ~typeBase() = default;
        virtual ptr_type create(CArgs... args) const = 0;
        virtual const std::type_info& getTypeInfo() const = 0;
    };

    //impl
    template<typename T>
    struct typeImpl : typeBase
    {
    public:
        virtual ~typeImpl() = default;
        virtual ptr_type create(CArgs... args) const override
            { return detail::make_unique_wrapper<Base, T, CArgs...>::call(args...); }
        virtual const std::type_info& getTypeInfo() const override { return typeid(T); };
    };

    using map_type = std::unordered_map<Identifier, const typeBase*>;
    using iterator = typename map_type::iterator;
    using const_iterator = typename map_type::const_iterator;

protected:
   map_type types_; //types stored

public:
    //get
    //id
    iterator getByIdentifier(const Identifier& id){ return types_.find(id); }
    const_iterator getByIdentifier(const Identifier& id) const { return types_.find(id); }

    //typeinfo
    iterator getByTypeInfo(const std::type_info& id)
    {
        auto it = types_.begin();
        for(; it != types_.end(); ++it)
            if(it->second->getTypeInfo() == id) break;
        return it;
    }
    const_iterator getByTypeInfo(const std::type_info& id) const
    {
        auto it = types_.cbegin();
        for(; it != types_.cend(); ++it)
            if(it->second->getTypeInfo() == id) break;
        return it;
    }

    //entry
    iterator getByEntry(const Identifier& id, const std::type_info& typeID)
        { auto it = getByTypeInfo(typeID); if(it != types_.end() && it->first == id) return it; return types_.end(); }
    const_iterator getByEntry(const Identifier& id, const std::type_info& typeID) const
        { auto it = getByTypeInfo(typeID); if(it != types_.cend() && it->first == id) return it; return types_.cend(); }

    bool valid(const const_iterator& it) const { return it != types_.end(); }
    bool valid(const iterator& it) const { return it != types_.end(); }

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T> std::size_t registerType(const Identifier& id)
    {
        types_[id] = new typeImpl<T>();
        return types_.size();
    }

    bool removeIdentifier(const Identifier& id)
    {
        auto it = getByIdentifier(id);
        if(it != types_.cend())
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool removeType(const T& obj = T{})
    {
        auto it = getByTypeInfo(typeid(obj));
        if(it != types_.cend())
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool removeEntry(const Identifier& id, const T& obj = T{})
    {
        auto it = getByEntry(id, typeid(obj));
        if(it != types_.cend())
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }

    bool identifierExists(const Identifier& id) const { return getByIdentifier(id) != types_.cend(); }
    bool typeExists( const std::type_info& typeID) const { return getByTypeInfo(typeID) != types_.cend(); }
    template<typename T> bool typeExists(const T& obj = T{}) const { return getByTypeInfo(typeid(obj)) != types_.cend(); }
    template<typename T> bool entryExists(const Identifier& id, const T& obj = T{}) const { return getByEntry(id, typeid(obj)) != types_.cend(); }
    bool entryExists(const Identifier& id, const std::type_info& typeID) const { return getByEntry(id, typeID) != types_.cend(); }

    ptr_type createObject(const Identifier& id, CArgs... args) const
        { auto it = getByIdentifier(id); if(it != types_.cend()) return it->second->create(args...); return nullptr; }
    ptr_type createObject(const std::type_info& id, CArgs... args) const
        { auto it = getByTypeInfo(id); if(it != types_.cend()) return it->second->create(args...); return nullptr; }

    const Identifier& getID(const std::type_info& typeID) const { auto it = getByTypeInfo(typeID); if(it != types_.cend()) return it->first; return Identifier{}; }
    template<typename T> const Identifier& getID(const T& obj = T{}) const { return getID(typeid(obj)); }

    const std::type_info& getTypeInfo(const Identifier& id) const { if(typeExists(id)) return types_[id].getTypeInfo(); return typeid(void); }
    const std::unordered_map<Identifier, const typeBase*>& getTypes() const { return types_; }
};

//registerFunc
template<typename T, typename Identifier, typename Base>
unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    return m.template registerType<T>(id);
}

}
