#pragma once

#include <string>

namespace util
{

class module;
class moduleLoader;
class loadedModule;

//module
class module
{
friend moduleLoader;

private:
    void* handle_; //used only from loader
    static module* module_;

public:
    module();

    virtual bool onLoad(moduleLoader& loader) = 0;
    virtual void onUnload() = 0;

    virtual unsigned int getVersion() const = 0;
    virtual unsigned int getTypeID() const = 0;

    static module* getObject() { return module_; };
};

//moduleLoader
class moduleLoader
{
public:
    module* load(const std::string& modName);
    void unload(module& mod);
};

}
