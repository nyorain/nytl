#pragma once

#include <string>

namespace nyutil
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
    virtual ~module(){}

    virtual bool onLoad(moduleLoader& loader) = 0;
    virtual void onUnload(){};

    static module* getObject() { return module_; };
};

//moduleLoader
class moduleLoader
{
public:
    virtual ~moduleLoader(){}

    virtual module* loadModule(const std::string& modName);
    virtual void unloadModule(module& mod);
};

}
