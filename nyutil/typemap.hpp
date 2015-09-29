#pragma once

#include <map>

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
    };

    //impl
    template<typename T>
    struct typeImpl : typeBase
    {
    public:
        virtual ~typeImpl() = default;
        virtual Base* create() const override { return new T(); }
    };

protected:
    std::map<Identifier, const typeBase*> types_;

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T>
    unsigned int registerType(const Identifier& id){ types_[id] = new typeImpl<T>(); return types_.size(); }

    Base* createObject(const Identifier& id){ return types_[id]->create(); } //todo: check for existence
};

//registerFunc
template<typename T, typename Identifier, typename Base>
constexpr unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    return m.template registerType<T>(id);
}

}
