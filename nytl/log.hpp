#pragma once

#include <nytl/misc.hpp>

#include <iostream>
#include <string>

namespace nytl
{

//
class logStream
{
public:
    std::string prefix {""};
    std::string name {"log"};
    std::ostream* stream {&std::cout};
    bool active {1};

public:
    logStream() = default;
    logStream(bool actv) : active(actv) {};
    logStream(std::ostream& os, bool actv = 1) : stream(&os), active(actv) {};
    logStream(const std::string& pre, const std::string& strm, std::ostream& str = std::cout, bool actv = 1)
        : prefix(pre), name(strm), stream(&str), active(actv) {}

    template<typename... Args> inline
    void output(Args... args) const { if(!active || !stream) return; printVars(*stream, prefix, name, ": ", args..., "\n"); }

    template<typename... Args> inline
    void operator()(Args... args) const { output(args...); }
};

//
namespace detail
{
//debug
#ifndef NDEBUG
    constexpr bool dEnabled = 0;
#else
    constexpr bool dEnabled = 1;
#endif // NDEBUG

//init
inline logStream& initDebug(){ static logStream str{"", "debug", std::clog, detail::dEnabled}; return str; }
inline logStream& initLog(){ static logStream str{"", "log", std::clog}; return str; }
inline logStream& initWarning(){ static logStream str{"", "warning", std::cout}; return str; }
inline logStream& initError(){ static logStream str{"", "warning", std::cout}; return str; }

}

//global
static logStream& sendDebug = detail::initDebug();
static logStream& sendLog = detail::initLog();
static logStream& sendWarning = detail::initWarning();
static logStream& sendError = detail::initError();

}
