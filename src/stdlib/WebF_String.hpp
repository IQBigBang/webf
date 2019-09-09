#ifndef WEBF_STRING_H
#define WEBF_STRING_H

#include <stdexcept>
#include "WebF_Type.h"
#include "WebF_Number.hpp"
#include "ExceptionHandler.h"

class WebF_String : IWebF_Type
{
public:
    WebF_String(std::string s) : s(s) {}

    bool implementsInterface(std::string iName)
    {
        return (iName == "iChainable" || iName == "iIndexable");
    }
    std::string getType()
    {
       return "str";
    }
    bool hasMethod(std::string methodName)
    {
        return (methodName == "++" || methodName == "chain"
                 || methodName == "at" || methodName == "len"
                 || methodName == "_cast_to_num" || methodName == "_cast_to_func"
                 || methodName == "_cast_to_str"); // _cast_to_str exists because of compatibility
    }
    IWebF_Type* invokeMethod(std::string methodName, std::vector<IWebF_Type*>* args)
    {
        if (methodName == "++" || methodName == "chain")
        {
            std::string str = "";
            for (IWebF_Type* a : *args) {
                WebF_String* casted = dynamic_cast<WebF_String*>(a->invokeMethod("_cast_to_str", nullptr));
                str += casted->getstr();
            }
            return new WebF_String(str);
        }
        if (methodName == "at")
        {
            // Argument count checking
            if (args->at(0)->getType() != "num")
            {
                ExceptionHandler::throwException(ExceptionType::InvalidType,
                                                 "Expected 'num' but found" + args->at(0)->getType());
            }
            WebF_Number* casted = dynamic_cast<WebF_Number*>(args->at(0));
            return new WebF_String(std::string(1, this->s[casted->getnum()]));
        }
        if (methodName == "len")
        {
            return new WebF_Number(this->s.size());
        }
        if (methodName == "_cast_to_str")
        {
            return this;
        }
        if (methodName == "_cast_to_num")
        {
            /*std::size_t res;
            try { std::stod(this->s, &res); }
            catch (const std::invalid_argument& ia)
            {
                ExceptionHandler::throwException(ExceptionType::InvalidFormat,
                                                 "Invalid format - string couldn't be casted to num");
            }
            return new WebF_Number(res);*/
        }
        if (methodName == "_cast_to_func")
        {
            // Big TODO
        }
        else ExceptionHandler::throwException(ExceptionType::NonexistentFunction,
                                              "Method " + methodName + " on type " + this->getType() + " doesn't exist");
    }

    std::string getstr()
    {
        return this->s;
    }

private:
    std::string s;
};

#endif // WEBF_STRING_H
