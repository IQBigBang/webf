#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include "AST.h"
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
        /// expr := (LPAREN object(type=Function) EQUALS object RPAREN)
        ///         | (LPAREN object {object} RPAREN)
        ExprNode* parse_expr();
        /// block := LBRACE {expr} RBRACE
        BlockNode* parse_block();
        /// list := LBRACKET {object} RBRACKET
        ListNode* parse_list();
        /// object := factor [DOT STRING]
        INode* parse_object();
        /// factor := ([DOLLAR] STRING)
        ///             | block
        ///             | list
        ///             | expr
        ///             | (LPAREN element RPAREN)
        INode* parse_factor();
        /// element := (TAGSTART STRING {attr} TAGCLOSINGEND)
        ///             | (TAGSTART STRING {attr} TAGEND {element_or_text} CLOSINGTAGSTART STRING TAGEND)
        ElementNode* parse_element();
        /// attr := STRING EQUAL (STRING | (DOLLAR STRING) | (DOLLAR object) | (LBRACE expr RBRACE))
        std::pair<std::string, INode*>* parse_attr();
        /// element_or_text := element | {STRING | (DOLLAR STRING) | (DOLLAR expr)}
        ElementNode* parse_elortext();

};

#endif // PARSER_H
