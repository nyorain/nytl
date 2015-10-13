#pragma once

#include <map>
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
    std::map<Identifier, const typeBase*> types_;

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T>
    unsigned int registerType(const Identifier& id){ types_[id] = new typeImpl<T>(); return types_.size(); }

    Base* createObject(const Identifier& id) const { auto it = types_.find(id); if(it != types_.end()) return it->second->create(); return nullptr; }
    bool typeExists(const Identifier& id) const { return types_.find(id) != types_.end(); }
    const std::type_info& getTypeInfo(const Identifier& id) const { if(typeExists(id)) return types_[id].getTypeInfo(); return std::type_info{}; }
};

//registerFunc
template<typename T, typename Identifier, typename Base>
constexpr unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    return m.template registerType<T>(id);
}

}
