#include <nyutil/module.hpp>

#include <dlfcn.h>

namespace nyutil
{

//proxies
extern "C" module* utilModuleLoadFunc_dl()
{
    return module::getObject();
}

typedef module* (*loadFunc)();

//module
module* module::module_ = nullptr;
module::module()
{
    if(module_)
    {
        //error! is set
        return;
    }

    module_ = this;
}

//moduleLoader
module* moduleLoader::load(const std::string& modName)
{
    void* handle = dlopen(modName.c_str(), RTLD_LAZY | RTLD_GLOBAL );
    if(!handle)
        return nullptr;

    loadFunc func = (loadFunc) dlsym(handle, "utilModuleLoadFunc_dl");
    if(!func)
        return nullptr;

    module* ret = func();
    if(ret)
    {
        ret->handle_ = handle;
        if(!ret->onLoad(*this))
        {
            unload(*ret);
            ret = nullptr;
        }
    }
    else
    {
        dlclose(handle);
    }

    return ret;
}

void moduleLoader::unload(module& mod)
{
    mod.onUnload();
    if(mod.handle_) dlclose(mod.handle_);
    //delete &mod;
}

}
