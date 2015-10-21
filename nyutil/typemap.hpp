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

#include <unordered_map>
#include <algorithm>
#include <typeinfo>

namespace nyutil
{

template<typename Identifier, typename Base = void>
class typemap
{
protected:
    //base
    struct typeBase
    {
    public:
        virtual ~typeBase() = default;
        virtual Base* create() const = 0;
        virtual const std::type_info& getTypeInfo() const = 0;
    };

    //impl
    template<typename T>
    struct typeImpl : typeBase
    {
    public:
        virtual ~typeImpl() = default;
        virtual Base* create() const override { return new T(); }
        virtual const std::type_info& getTypeInfo() const override { return typeid(T); };
    };

protected:
    std::unordered_map<Identifier, const typeBase*> types_;

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T> std::size_t registerType(const Identifier& id) { types_[id] = new typeImpl<T>(); return types_.size(); }
    Base* createObject(const Identifier& id) const { auto it = types_.find(id); if(it != types_.end()) return it->second->create(); return nullptr; }
    bool typeExists(const Identifier& id) const { return types_.find(id) != types_.end(); }
    const std::type_info& getTypeInfo(const Identifier& id) const { if(typeExists(id)) return types_[id].getTypeInfo(); return typeid(void); }
};

//registerFunc
template<typename T, typename Identifier, typename Base>
constexpr unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    return m.template registerType<T>(id);
}

}
