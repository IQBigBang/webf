#include "Lexer.h"
#include "ExceptionHandler.h"
#include <sstream>

Lexer::Lexer(std::string code)
{
    this->code = code;
    this->pos = 0;
    this->lineno = 1;
    this->linepos = 0;
    this->currentT = Token {isEmpty: true};
    this->afterCurrentT = Token {isEmpty: true};

    this->oneCharTokens['\x01'] = Eof;
    this->oneCharTokens['('] = LPAREN;
    this->oneCharTokens[')'] = RPAREN;
    this->oneCharTokens['['] = LBRACKET;
    this->oneCharTokens[']'] = RBRACKET;
    this->oneCharTokens['{'] = LBRACE;
    this->oneCharTokens['}'] = RBRACE;
    this->oneCharTokens['$'] = DOLLAR;
    this->oneCharTokens['.'] = DOT;
    this->oneCharTokens['='] = EQUAL;
    this->oneCharTokens['>'] = TAGEND;

}

char Lexer::getCh()
{
    if (this->pos >= this->code.length())
        return '\x01';
    else
        return this->code[this->pos];
}

void Lexer::skipWhitespace()
{
    while (std::isspace(this->getCh()))
    {
        if (this->getCh() == '\n') // TODO: Add multiplatform newline (\r, \r\n) (see testcases)
        {
            this->lineno++;
            this->linepos = 0;
        }
        else
            this->linepos++;
        this->pos++;
    }
}

Token Lexer::next()
{
    this->skipWhitespace();
    char c = this->getCh();
    auto i = this->oneCharTokens.find(c);
    if (i != this->oneCharTokens.end())
    {
        // TODO: Maybe optimize the map usage?
        std::string str(1, c);
        this->pos++;
        return Token {isEmpty: false, ttype: i->second, tval: str};
    }
    if (c == '<')
    {
        this->pos++;
        if (this->getCh() == '/')
        {
            this->pos++;
            return Token {isEmpty: false, ttype: CLOSINGTAGSTART, tval: "</"};
        }
        else
        {
            return Token {isEmpty: false, ttype: TAGSTART, tval: "<"};
        }
    }
    if (c == '/')
    {
        this->pos++;
        if (this->getCh() == '>')
        {
            this->pos++;
            return Token {isEmpty: false, ttype: TAGCLOSINGEND, tval: "/>"};
        }
    }
    if (c == '"')
    {
        this->pos++; // Skip frist quote
        c = this->getCh();
        std::string s = "";
        while (c != '"')
        {
            this->pos++;
            this->linepos++;
            if (c == '\n')   // TODO: mutliplatform newline, possibly make a function to avoid repetitive code
            {
                this->lineno++;
                this->linepos = 0;
            }
            s.append(1, c);
            c = this->getCh();
        }
        this->pos++; // Skip the last quote
        return Token {isEmpty: false, ttype: STRING, tval: s};
    }
    // else it's a non-quoted string
    std::string s = "";
    while (this->nameTerminatingChars.find(c) == std::string::npos)
    {
        this->pos++;
        this->linepos++;
        s.append(1, c);
        c = this->getCh();
    }
    return Token {isEmpty: false, ttype: STRING, tval: s};
}

bool Lexer::peek(TokenType expected_type)
{
    if (this->currentT.isEmpty) this->currentT = next();
    return this->currentT.ttype == expected_type;
}

bool Lexer::peekNext(TokenType expected_type)
{
    if (this->afterCurrentT.isEmpty) {
        if (this->currentT.isEmpty) {
            this->currentT = next();
        }
        this->afterCurrentT = next();
    }
    return this->afterCurrentT.ttype == expected_type;
}

std::string Lexer::eat()
{
    if (this->currentT.isEmpty) {
        this->currentT = this->next();
    }
    std::string token_text = this->currentT.tval;
    if (this->afterCurrentT.isEmpty) {
        this->currentT = next();
    } else {
        this->currentT = this->afterCurrentT;
        this->afterCurrentT = Token {isEmpty: true};
    }
    return token_text;
}

std::string Lexer::expect(TokenType expected_type)
{
    if (!this->peek(expected_type)) {
        std::stringstream ss;
        ss << "Unexpected token at " + this->lineno << ", " << this->linepos;
        ExceptionHandler::throwException(ExceptionType::UnexpectedToken, ss.str());
    }
    return this->eat();
}

Lexer::~Lexer()
{
    //dtor
}
