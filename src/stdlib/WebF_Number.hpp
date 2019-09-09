#ifndef WEBF_NUMBER_H
#define WEBF_NUMBER_H

#include "WebF_Type.h"
#include "WebF_String.hpp"
#include "ExceptionHandler.h"

class WebF_Number : public IWebF_Type
{
public:
    WebF_Number(double d) : d(d) {}

    bool implementsInterface(std::string iName)
    {
        return (iName == "iAddable" || iName == "iAlgebraic");
    }

    std::string getType()
    {
       return "num";
    }

    bool hasMethod(std::string methodName)
    {
        return (methodName == "+" || methodName == "add"
                 || methodName == "-" || methodName == "sub"
                 || methodName == "*" || methodName == "mul"
                 || methodName == "/" || methodName == "div"
                 || methodName == "_cast_to_str" || methodName == "_cast_to_num");
    }

    IWebF_Type* invokeMethod(std::string methodName, std::vector<IWebF_Type*>* args)
    {
        if (methodName == "+" || methodName == "add")
        {
            double res = this->d;
            for (IWebF_Type* a : *args) {
                if (a->hasMethod("_cast_to_num"))
                    res += (dynamic_cast<WebF_Number*>(a->invokeMethod("_cast_to_num", nullptr)))->getnum();
                else ExceptionHandler::throwException(ExceptionType::MissingCast, "Missing cast to num");

            }
            return new WebF_Number(res);
        }
        if (methodName == "-" || methodName == "sub")
        {
            double res = this->d;
            for (IWebF_Type* a : *args) {
                if (a->hasMethod("_cast_to_num"))
                    res -= (dynamic_cast<WebF_Number*>(a->invokeMethod("_cast_to_num", nullptr)))->getnum();
                else ExceptionHandler::throwException(ExceptionType::MissingCast, "Missing cast to num");

            }
            return new WebF_Number(res);
        }
        if (methodName == "*" || methodName == "mul")
        {
            double res = this->d;
            for (IWebF_Type* a : *args) {
                if (a->hasMethod("_cast_to_num"))
                    res *= (dynamic_cast<WebF_Number*>(a->invokeMethod("_cast_to_num", nullptr)))->getnum();
                else ExceptionHandler::throwException(ExceptionType::MissingCast, "Missing cast to num");

            }
            return new WebF_Number(res);
        }
        if (methodName == "/" || methodName == "div")
        {
            double res = this->d;
            for (IWebF_Type* a : *args) {
                if (a->hasMethod("_cast_to_num"))
                    res /= (dynamic_cast<WebF_Number*>(a->invokeMethod("_cast_to_num", nullptr)))->getnum();
                else ExceptionHandler::throwException(ExceptionType::MissingCast, "Missing cast to num");

            }
            return new WebF_Number(res);
        }
        if (methodName == "_cast_to_num") return this;
/*        if (methodName == "_cast_to_str") return new WebF_String(std::to_string(this->d));
        else ExceptionHandler::throwException(ExceptionType::NonexistentFunction,
                                          "Method " + methodName + " on type " + this->getType() + " doesn't exist");*/
    }

    double getnum()
    {
        return this->d;
    }

private:
    double d;
};

#endif // WEBF_NUMBER_H
