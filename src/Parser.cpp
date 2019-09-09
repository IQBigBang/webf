#include "Parser.h"
#include "ExceptionHandler.h"
#include "Lexer.h"
#include "INode.h"
// Nodes
#include "BlockNode.hpp"
#include "ElementNode.hpp"
#include "ExprNode.hpp"
#include "ListNode.hpp"
#include "MemberAccessNode.hpp"
#include "NumberNode.hpp"
#include "StringNode.hpp"
#include "VariableNode.hpp"
#include "VardefNode.hpp"

Parser::Parser(Lexer& l) : l(l)
{
    //ctor
}

ExprNode* Parser::parse_expr()
{
    this->l.expect(LPAREN);
    if (this->l.peek(NAME) && this->l.peekNext(EQUAL))
    {
        VardefNode* varname = new VardefNode(this->l.eat());
        this->l.eat(); // EQUAL
        INode* val = this->parse_object();
        this->l.expect(RPAREN);
        auto args = new std::vector<INode*>();
        args->push_back(varname);
        args->push_back(val); 
        return new ExprNode(new VariableNode("set"), args);
    }
    INode* func = this->parse_object();
    auto args = new std::vector<INode*>();
    while (!this->l.peek(RPAREN))
    {
        args->push_back(this->parse_object());
    }
    this->l.eat(); // RPAREN
    return new ExprNode(func, args);
}

BlockNode* Parser::parse_block()
{
    this->l.expect(LBRACE);
    auto expressions = new std::vector<ExprNode*>;
    while (!this->l.peek(RBRACE))
    {
        expressions->push_back(this->parse_expr());
    }
    this->l.eat(); // RBRACE
    return new BlockNode(expressions);
}

ListNode* Parser::parse_list()
{
    this->l.expect(LBRACKET);
    auto contents = new std::vector<INode*>;
    while (!this->l.peek(RBRACKET))
    {
        contents->push_back(this->parse_object());
    }
    this->l.eat(); // RBRACKET
    return new ListNode(contents);
}

INode* Parser::parse_object()
{
    auto a = this->parse_atom();
    if (!this->l.peek(DOT)) return a; // No member access
    std::vector<std::string> members; 
    while (this->l.peek(DOT))
    {
        this->l.eat(); // DOT
        members.push_back(this->l.expect(NAME));
    }
    INode* last = a;
    while (members.size() != 0)
    {
        last = new MemberAccessNode(last, members[0]);
        members.erase(members.begin());
    }
    return last;
}

INode* Parser::parse_atom()
{
    if (this->l.peek(STRING)) return new StringNode(this->l.eat());
    if (this->l.peek(NUMBER)) return new NumberNode(std::stod(this->l.eat()));
    if (this->l.peek(NAME)) return new VariableNode(this->l.eat());
    if (this->l.peek(LBRACKET)) return this->parse_list();
    if (this->l.peek(LBRACE)) return this->parse_block();
    if (this->l.peek(TAGSTART)) return this->parse_element();
    if (this->l.peek(DOLLAR)) {
        this->l.eat();
        return new VardefNode(this->l.expect(NAME));
    }
    return this->parse_expr();
}

ElementNode* Parser::parse_element()
{
    this->l.expect(TAGSTART);
    std::string elname = this->l.expect(NAME);
    std::map<std::string, INode*>* attrs = new std::map<std::string, INode*>();
    while (!this->l.peek(TAGEND) && !this->l.peek(CLOSINGTAGEND)) {
        std::pair<std::string, INode*>* attr = this->parse_attr();
        attrs->insert(*attr);
    }
    if (this->l.peek(CLOSINGTAGEND)) {
        this->l.eat(); // CLOSINGTAGEND
        return new ElementNode(elname, attrs, new std::vector<ElementNode*>());
    }
    this->l.expect(TAGEND);
    auto children = new std::vector<ElementNode*>();
    while (true) {
        std::string r = this->l.readRawUntil('<');
        if (r == "")
        {
            if (this->l.peek(CLOSINGTAGSTART)) break;
            children->push_back(this->parse_element());
        } else children->push_back(new TextNode(r));
    }
    this->l.expect(CLOSINGTAGSTART);
    std::string closingTagName = this->l.expect(NAME);
    if (closingTagName != elname) {
        ExceptionHandler::throwException(ExceptionType::TagNamesNotEqual);
    }
    this->l.expect(TAGEND);
    return new ElementNode(elname, attrs, children);
}

std::pair<std::string, INode*>* Parser::parse_attr()
{
    auto attr = new std::pair<std::string, INode*>();
    attr->first = this->l.expect(NAME);
    this->l.expect(EQUAL);
    attr->second = this->parse_object();
    return attr;
}