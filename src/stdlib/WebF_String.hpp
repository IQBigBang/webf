/*#ifndef WEBF_STRING_H
#define WEBF_STRING_H

#include <stdexcept>
#include "WebF_Type.h"
#include "WebF_Number.hpp"
#include "ExceptionHandler.h"

class WebF_String : public IWebF_Type
{
public:
    WebF_String(std::string s) : s(s) {}

    bool implementsInterface(std::string iName)
    {
        return (iName == "iChainable" || iName == "iIndexable" || iName == "iCastable<str>" || iName == "iCastable<num>");
    }

    std::string getType()
    {
       return "str";
    }

    IWebF_Type* invokeMethod(std::string methodName, std::vector<IWebF_Type*>* args)
    {
        if (methodName == "++" || methodName == "chain")
        {
            std::string str = "";
            for (IWebF_Type* a : *args) {
                WebF_String* casted = dynamic_cast<WebF_String*>(a->invokeMethod("str", nullptr));
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
        if (methodName == "cast_str")
        {
            return this;
        }
        if (methodName == "cast_num")
        {
            try {
                return new WebF_Number(std::stod(this->s));
            }
            catch (const std::invalid_argument& ia)
            {
                ExceptionHandler::throwException(ExceptionType::InvalidFormat,
                                                 "Invalid format - string couldn't be casted to num");
            }
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

#endif // WEBF_STRING_H*/
