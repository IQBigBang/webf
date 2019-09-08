#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include "INode.h"
#include "ExprNode.hpp"
#include "BlockNode.hpp"
#include "ElementNode.hpp"
#include "ListNode.hpp"
#include "Lexer.h"

class Parser
{
    public:
        Parser(Lexer &l);
        INode* parseLine();

    #ifndef WEBF_TESTING
    protected:

    private:
    #endif
        Lexer& l;
        /// expr = (LPAREN, object, {object}, RPAREN) | (LPAREN, object, EQUAL, object, RPAREN)
        ExprNode* parse_expr();
        /// block = LBRACE, {expr}, RBRACE
        BlockNode* parse_block();
        ///list = LBRACKET, {object}, RBRACKET
        ListNode* parse_list();
        /// object = atom | (object, DOT, NAME)
        INode* parse_object();
        /// atom = STRING | NUMBER | NAME | (DOLLAR, NAME) | expr | list | block | element
        INode* parse_atom();
        /// element = (TAGSTART, NAME, {attr}, CLOSINGTAGEND) 
        ///         | (TAGSTART, NAME, {attr}, TAGEND, {text | element}, CLOSINGTAGSTART, NAME, TAGEND)
        ElementNode* parse_element();
        /// attr := NAME, EQUAL object
        std::pair<std::string, INode*>* parse_attr();

};

#endif // PARSER_H
