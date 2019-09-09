#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include <string>
#include <iostream>

enum ExceptionType
{
    // Lexer errors
    UnexpectedToken = 102,
    // Parser errors
    TagNamesNotEqual = 113,
    // Runtime errors
    MissingCast = 142,
    NonexistentFunction = 143,
    InvalidType = 144,
    InvalidFormat = 145
};


class ExceptionHandler
{
    public:
        static void throwException(ExceptionType et);
        static void throwException(ExceptionType et, std::string msg);

    private:
        ExceptionHandler();
};

#endif // EXCEPTIONHANDLER_H
