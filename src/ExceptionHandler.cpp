#include "ExceptionHandler.h"

void ExceptionHandler::throwException(ExceptionType et)
{
    std::string defaultMsg;
    switch (et)
    {
    case ExceptionType::UnexpectedToken:
        defaultMsg = "Unexpected token";
        break;
    case ExceptionType::TagNamesNotEqual:
        defaultMsg = "Tag names not equal";
        break;
    case ExceptionType::MissingCast:
        defaultMsg = "Missing cast function, couldn't cast to required type";
        break;
    case ExceptionType::InvalidFormat:
        defaultMsg = "Invalid format";
        break;
    case ExceptionType::InvalidType:
        defaultMsg = "Invalid type";
        break;
    case ExceptionType::NonexistentFunction:
        defaultMsg = "Attempt to call a function that is not defined";
        break;
    default:
        defaultMsg = "Uknown error";
    }
    ExceptionHandler::throwException(et, defaultMsg);
}

void ExceptionHandler::throwException(ExceptionType et, std::string msg)
{
    std::cerr << "Error: " << msg << std::endl;
    throw (int)et;
}
