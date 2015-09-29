#pragma once

namespace nyutil
{

template<typename Identifier, typename Base = void>#
class typemap
{
protected:
    //base
    struct typeBase
    {
    public:
        virtual Base* create() const = 0;
    };

    //impl
    template<typename T>
    struct typeImpl : typeBase
    {
    public:
        virtual Base* create() const override { return new T(); }
    };
protected:
    std::map<Identifier, typeBase*> types_;

public:
    ~typemap(){ for(auto& val : types_) delete val.second; }

    template<typename T>
    unsigned int registerType(const Identifier& id){ types_[id] = new typeImpl<T>(); }

    Base* createObject(const Identifier& id){ return new types_[id]; } //todo: check for existence
};

//registerFunc
template<typename T, typename Identifier, typename Base>
unsigned int registerType(typemap<Identifier, Base>& m, const Identifier& id)
{
    m.template registerType<T>(id);
}

}
