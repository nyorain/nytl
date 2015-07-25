#pragma once

#include <string>
#include <vector>
#include <functional>

namespace nyutil
{

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

    static const std::string defaultShort_;
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

    void addInput(int argc, const char** argv);
    void addInput(const std::string& str);
    void addInput(const std::vector<std::string>& stringVec);
    unsigned char parse(int argc = 0, const char** argv = nullptr); //calls addInput on arguments

    void addFlag(std::string name, bool& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = defaultShort_);
    void addStringVar(std::string name, std::string& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = defaultShort_);
    void addIntVar(std::string name, int& ret, const std::string& help = "", const std::string& category = "core", std::string shortArg = defaultShort_);

    void onMalformedToken(std::function<void(std::string)> func);
};

}
