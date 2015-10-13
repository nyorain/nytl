#pragma once

#include <memory>
#include <functional>

namespace nyutil
{

#if __cplusplus >= 201402L
 using std::make_unique;
#else

template<typename T, class... Args >
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
std::unique_ptr<T> make_unique(std::size_t size)
{
    return std::unique_ptr<T>(new typename std::remove_extent<T>::type[size]());
}

#endif // __cplusplus
}
