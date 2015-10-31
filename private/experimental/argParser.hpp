#pragma once

#include <nyutil/misc.hpp>

#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace nytl
{

//todo: use std::regex instead of custom parsing

//argParser
class argParser
{
private:

    //arg
    struct arg
    {
        enum argType
        {
            flag,
            intVar,
            stringVar
        };

        arg(const std::string& longA, const std::string& shortA, bool& v, const std::string& help, const std::string& category) : type_(flag), bool_(&v), category_(category), help_(help), long_(longA), short_(shortA) {};
        arg(const std::string& longA, const std::string& shortA, std::string& v, const std::string& help, const std::string& category) : type_(stringVar), string_(&v), category_(category), help_(help), long_(longA), short_(shortA) {};
        arg(const std::string& longA, const std::string& shortA, int& v, const std::string& help, const std::string& category) : type_(intVar), int_(&v), category_(category), help_(help), long_(longA), short_(shortA) {};

        arg(const arg& other);
        arg& operator=(const arg& other);


        argType type_ = flag;
        union
        {
            bool* bool_;
            std::string* string_;
            int* int_;
        };

        std::string category_;
        std::string help_;

        std::string long_;
        std::string short_;

        bool operator==(const std::string& str) const
        {
            return ((!str.empty()) && ((long_ == str) || (short_ == str)));
        }
    };

    //expr
    struct expr
    {
        enum exprType
        {
            flag,
            var,
            malformed
        };

        exprType type_ = malformed;

        std::string original_;
        std::string flag_; //contains the flag
        std::string var_; //contains var
    };

    static bool fillArg(arg& a, const expr& e);

public:
    enum parseReturn : unsigned char
    {
        helpCalled = 1, //defines if help flag was called
        malformedToken = 2 //defined if there was any malformed token
    };

protected:
    std::vector<arg> args_;
    std::vector<expr> input_;

    std::function<void(std::string)> malformedTokenFunc_;

public:
    argParser();

    inline void addInput(int argc, const char** argv);
    inline void addInput(const std::string& str);
    inline void addInput(const std::vector<std::string>& stringVec);
    inline unsigned char parse(int argc = 0, const char** argv = nullptr); //calls addInput on arguments

    inline void addFlag(std::string name, bool& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = "");
    inline void addStringVar(std::string name, std::string& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = "");
    inline void addIntVar(std::string name, int& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = "");

    inline void onMalformedToken(std::function<void(std::string)> func);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

inline void defaultMalformedToken(const std::string& str)
{
    std::cout << "unknown or malformed token " << str << std::endl;
}

bool argParser::fillArg(arg& a, const expr& e)
{
    if(e.type_ == expr::malformed || e.flag_.empty() || ((a.short_ != e.flag_) && (a.long_ != e.flag_)))
        return 0;

    if(e.type_ == expr::var)
    {
        if(a.type_ == arg::stringVar)
        {
            *a.string_ = e.var_;
            return 1;
        }
        else if(a.type_ == arg::intVar)
        {
            int backup = *a.int_; //needed?

            try
            {
                *a.int_ = std::stoi(e.var_);
            }
            catch(const std::exception& err)
            {
                *a.int_ = backup;
                return 0;
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }

    if(e.type_ == expr::flag)
    {
        if(a.type_ == arg::flag)
        {
            *a.bool_ = 1;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    //should never happen?
    return 0;
}

//argParser
argParser::argParser() : malformedTokenFunc_(&defaultMalformedToken)
{

}

void argParser::addInput(int argc, const char** argv)
{
    std::vector<std::string> ret;
    for(unsigned int i(0); i < (unsigned int) argc; i++)
    {
        ret.push_back(argv[i]);
    }
    addInput(ret);
}

void argParser::addInput(const std::string& str)
{
    addInput(split(str, ' '));
}

void argParser::addInput(const std::vector<std::string>& strVec)
{
    unsigned int i = 0;
    while(i < strVec.size())
    {
        expr ret;
        ret.original_ = strVec[i];

        if(strVec[i].substr(0, 1) == "-") //is valid
        {
            auto vec = split(strVec[i], '=');
            if(vec.size() == 1) //flag, search for vars given just with space
            {
                if(i + 1 >= strVec.size() || strVec[i + 1].substr(0, 1) == "-") //is just a flag
                {
                    ret.type_ = expr::flag;
                    ret.flag_ = strVec[i];
                }
                else
                {
                    ret.type_ = expr::var;
                    ret.flag_ = strVec[i];

                    i++;
                    ret.var_ += strVec[i]; //next one is var
                    ret.original_ += " " + strVec[i];
                    i++;

                    while(i < strVec.size() && strVec[i].substr(0, 1) != "-") //append all next values which are NOT flags to the var
                    {
                        ret.var_ += " " + strVec[i];
                        ret.original_ += " " + strVec[i];
                        i++;
                    }
                }
            }
            else if(vec.size() == 2)
            {
                ret.type_ = expr::var;
                ret.flag_ = vec[0];
                ret.var_ = vec[1];
            }
            else if(vec.size() > 2)
            {
                ret.type_ = expr::malformed;
            }
        }
        else
        {
            ret.type_ = expr::malformed;
        }

        input_.push_back(ret);
        i++;
    }
}

unsigned char argParser::parse(int argc, const char** argv)
{
    addInput(argc, argv);

    unsigned char ret = 0;
    bool handled = 0;
    bool help = 0;

    args_.insert(args_.begin(), arg("--help", "-h", help, "see this help output", "core"));

    for(auto& e : input_)
    {
        if(e.flag_.empty())
            continue;

        handled = 0;
        for(auto& a : args_)
        {
            if(fillArg(a, e))
                handled = 1;
        }

        if(!handled)
        {
            if(malformedTokenFunc_) malformedTokenFunc_(e.original_);
            ret |= malformedToken;
            help = 1;
        }

    }

    if(help)
    {
        //todo: categories

        ret |= help;

        std::cout << "arguments: " << std::endl;
        for(auto& a : args_)
        {
            std::cout << "  " << a.short_ << "\t" << a.long_ << "\t\t" << a.help_ << std::endl;
        }
    }

    return ret;
}

void argParser::addFlag(std::string name, bool& ret, const std::string& help, const std::string& category, std::string shortArg)
{
    if(name.substr(0, 1) != "-") name.insert(0, "-");
    if(name.substr(1, 1) != "-") name.insert(0, "-");

    if(shortArg.empty())
    {
        shortArg = name.substr(1, 2);
    }

    args_.push_back(arg(name, shortArg, ret, help, category));
}

void argParser::addStringVar(std::string name, std::string& ret, const std::string& help, const std::string& category, std::string shortArg)
{
    if(name.substr(0, 1) != "-") name.insert(0, "-");
    if(name.substr(1, 1) != "-") name.insert(0, "-");

    if(shortArg.empty())
    {
        shortArg = name.substr(1, 2);
    }

    arg a(name, shortArg, ret, help, category);
    args_.push_back(a);
}

void argParser::addIntVar(std::string name, int& ret, const std::string& help, const std::string& category, std::string shortArg)
{
    if(name.substr(0, 1) != "-") name.insert(0, "-");
    if(name.substr(1, 1) != "-") name.insert(0, "-");

    if(shortArg.empty())
    {
        shortArg = name.substr(1, 2);
    }

    args_.push_back(arg(name, shortArg, ret, help, category));
}

void argParser::onMalformedToken(std::function<void(std::string)> func)
{
    malformedTokenFunc_ = func;
}

//arg
argParser::arg::arg(const arg& other) :
    type_(other.type_),
    bool_(nullptr),
    category_(other.category_),
    help_(other.help_),
    long_(other.long_),
    short_(other.short_)
{
    if(type_ == flag)bool_ = other.bool_;
    else if(type_ == stringVar) string_ = other.string_;
    else int_ = other.int_;
}

argParser::arg& argParser::arg::operator=(const argParser::arg& other)
{
    type_ = other.type_;
    category_ = other.category_;
    help_ = other.help_;
    long_ = other.long_;
    short_ = other.short_;

    if(type_ == flag)bool_ = other.bool_;
    else if(type_ == stringVar) string_ = other.string_;
    else int_ = other.int_;

    return *this;
}

}
