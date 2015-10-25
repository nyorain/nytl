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

#include <nyutil/make_unique.hpp>

#include <unordered_map>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include <type_traits>

namespace nyutil
{

namespace detail
{

template<typename Base, typename T> struct make_unique_wrapper
{
    static std::unique_ptr<Base> call() { return make_unique<T>(); };
};

template<typename T> struct make_unique_wrapper<void, T>
{
    static void* call() { return new T(); };
};

}


//typemap
template<typename Identifier, typename Base = void>
class typemap
{
public:
    using ptr_type = typename std::conditional<std::is_same<Base, void>::value, void*, std::unique_ptr<Base>>::type;

    //base
    struct typeBase
    {
    public:
        virtual ~typeBase() = default;
        virtual ptr_type create() const = 0;
        virtual const std::type_info& getTypeInfo() const = 0;
    };

    //impl
    template<typename T>
    struct typeImpl : typeBase
    {
    public:
        virtual ~typeImpl() = default;
        virtual ptr_type create() const override { return detail::make_unique_wrapper<Base, T>::call(); }
        virtual const std::type_info& getTypeInfo() const override { return typeid(T); };
    };

protected:
    std::unordered_map<Identifier, const typeBase*> types_;

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T> std::size_t registerType(const Identifier& id) { types_[id] = new typeImpl<T>(); return types_.size(); }

    bool removeIdentifier(const Identifier& id)
    {
        auto it = types_.find(id);
        if(it != types_.cend())
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool removeType(const T& obj = T{})
    {
        for(auto it = types_.cbegin(); it != types_.cend(); ++it)
        {
            if(it->second->getTypeInfo() == typeid(obj))
            {
                types_.erase(it);
                return 1;
            }
        }
        return 0;
    }
    template<typename T> bool removeEntry(const Identifier& id, const T& obj = T{})
    {
        auto it = types_.find(id);
        if(it != types_.cend())
        {
            if(it->second->getTypeInfo() == typeid(obj))
            {
                types_.erase(it);
                return 1;
            }
        }
        return 0;
    }

    bool identifierExists(const Identifier& id) const { return types_.find(id) != types_.end(); }
    template<typename T> bool typeExists(const T& obj = T{}) const
    {
        for(auto it = types_.cbegin(); it != types_.cend(); ++it)
            if(it->second->getTypeInfo() == typeid(obj))
                return 1;

        return 0;
    }
    template<typename T> bool entryExists(const Identifier& id, const T& obj = T{}) const
    {
        auto it = types_.find(id);
        if(it != types_.cend())
            if(it->second->getTypeInfo() == typeid(obj))
                return 1;

        return 0;
    }

    ptr_type createObject(const Identifier& id) const { auto it = types_.find(id); if(it != types_.end()) return it->second->create(); return nullptr; }
    const std::type_info& getTypeInfo(const Identifier& id) const { if(typeExists(id)) return types_[id].getTypeInfo(); return typeid(void); }

    const std::unordered_map<Identifier, const typeBase*>& getTypes() const { return types_; }
};

//registerFunc
template<typename T, typename Identifier, typename Base>
constexpr unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    return m.template registerType<T>(id);
}

}
