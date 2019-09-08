#ifndef LEXER_H
#define LEXER_H

#include <string> // string
#include <cctype> // isspace
#include <map> // map
#include <iostream> // cout endl

enum TType
{
    STRING,
    LPAREN, // (
    RPAREN, // )
    LBRACKET, // [
    RBRACKET, // ]
    LBRACE, // {
    RBRACE, // }
    DOT, // .
    DOLLAR, // $
    EQUAL, // =
    TAGSTART, // <
    TAGEND, // >
    TAGCLOSINGEND, // />
    CLOSINGTAGSTART, // </
    Eof // ALL-CAPS EOF is a macro
};

struct Token
{
    bool isEmpty;
    TType ttype;
    std::string tval;
};


class Lexer
{
public:
    Lexer(std::string code);
    virtual ~Lexer();
    bool peek(TType expected_type);
    bool peekNext(TType expected_type);
    std::string eat();
    std::string expect(TType expected_type);

    // TODO: Change this macro definition, it prevents CodeBlocks from correctly parsing public / private
#ifndef WEBF_TESTING // WEBF_TESTING makes all members public for debugging purposes
protected:

private:
#endif
    std::string code;
    unsigned int pos;
    // DEBUG INFO
    int lineno;
    int linepos;
    // TO PREVENT REPETITIVE LEXING
    Token currentT;
    Token afterCurrentT;

    // for simplicity
    std::map<char, TType> oneCharTokens;
    std::string nameTerminatingChars = "()[]{}.<>$= /\t\n\x01";

    /**
    Safely get current character in code
    Prevents access outside the string
    */
    char getCh();

    /**
    Skips all whitespace characters (space, tab, newline)
    */
    void skipWhitespace();

    /**
    Lex the next token
    */
    Token next();
};

#endif // LEXER_H
