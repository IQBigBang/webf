#include "Parser.h"
#include "AST.h"

Parser::Parser(Lexer& l) : l(l)
{
    //ctor
}

ExprNode* Parser::parse_expr()
{
    this->l.expect(LPAREN);
    INode* func = this->parse_object();
    if (this->l.peek(EQUAL))
    {
        this->l.eat(); // EQUAL
        INode* val = this->parse_object();
        this->l.expect(RPAREN);
        auto args = new std::vector<INode*>();
        args->push_back(func);
        args->push_back(val);
        return new ExprNode(new StringNode("set"), args);
    }
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
    auto f = this->parse_factor();
    if (this->l.peek(DOT)) {
        this->l.eat(); // DOT
        std::string methodName = this->l.expect(STRING);
        return new InstanceAccessNode(f, methodName);
    }
    return f;
}

INode* Parser::parse_factor()
{
    if (this->l.peek(LBRACKET)) return this->parse_list();
    if (this->l.peek(LBRACE)) return this->parse_block();
    if (this->l.peek(LPAREN)) {
        if (this->l.peekNext(TAGSTART)) {
            this->l.eat(); // LPAREN
            auto el = this->parse_element();
            this->l.expect(RPAREN);
            return el;
        }
        return this->parse_expr();
    }
    if (this->l.peek(DOLLAR)) {
        this->l.eat(); // DOLLAR
        auto args = new std::vector<INode*>();
        args->push_back(new StringNode(this->l.expect(STRING)));
        return new ExprNode(new StringNode("get"), args);
    }
    return new StringNode(this->l.expect(STRING));
}

ElementNode* Parser::parse_element()
{
    this->l.expect(TAGSTART);
    std::string elname = this->l.expect(STRING);
    std::map<std::string, INode*>* attrs = new std::map<std::string, INode*>();
    while (!this->l.peek(TAGEND) && !this->l.peek(TAGCLOSINGEND)) {
        std::pair<std::string, INode*>* attr = this->parse_attr();
        attrs->insert(*attr);
    }
    if (this->l.peek(TAGCLOSINGEND)) {
        this->l.eat(); // TAGCLOSINGEND
        return new ElementNode(elname, attrs, new std::vector<ElementNode*>());
    }
    this->l.expect(TAGEND);
    auto children = new std::vector<ElementNode*>();
    while (!this->l.peek(CLOSINGTAGSTART)) {
        children->push_back(this->parse_elortext());
    }
    this->l.expect(CLOSINGTAGSTART);
    std::string closingTagName = this->l.expect(STRING);
    if (closingTagName != elname) {
        std::cout << "Tag names are not equal" << std::endl;
        throw 113;
    }
    this->l.expect(TAGEND);
    return new ElementNode(elname, attrs, children);
}

std::pair<std::string, INode*>* Parser::parse_attr()
{
    auto attr = new std::pair<std::string, INode*>();
    attr->first = this->l.expect(STRING);
    this->l.expect(EQUAL);
    if (this->l.peek(STRING)) {
        auto static_attr_value = this->l.eat(); // STRING
        attr->second = new StringNode(static_attr_value);
        return attr;
    }
    if (this->l.peek(DOLLAR)) {
        this->l.eat(); // DOLLAR
        if (this->l.peek(STRING)) {
            // Exception: attr1 = $x is treated as attr1 = $(get x)
            // Without this exception you'd have to write $$x which is unintuitive
            auto args = new std::vector<INode*>();
            args->push_back(new StringNode(this->l.eat()));
            attr->second = new ExprNode(new StringNode("get"), args);
            return attr;
        }
        attr->second = this->parse_object();
        return attr;
    }
    this->l.expect(LBRACE);
    auto lazy_expr = new std::vector<ExprNode*>();
    lazy_expr->push_back(this->parse_expr());
    this->l.expect(RBRACE);
    attr->second = new BlockNode(lazy_expr);
    return attr;
}

ElementNode* Parser::parse_elortext()
{
    if (this->l.peek(TAGSTART)) {
        return this->parse_element();
    }
    auto textnodes = new std::vector<INode*>();
    while (!this->l.peek(CLOSINGTAGSTART)) {
        if (this->l.peek(DOLLAR)) {
            this->l.eat(); // DOLLAR
            if (this->l.peek(STRING)) {
                auto args = new std::vector<INode*>();
                args->push_back(new StringNode(this->l.eat()));
                textnodes->push_back(new ExprNode(new StringNode("get"), args));
            } else {
                textnodes->push_back(this->parse_expr());
            }
        } else {
            // Temporary solution: convert every special token back to text
            // Better solution TODO: enable 'text mode' for lexer
            // in text mode lexer would ignore all special chars except for dollar and tags
           std::string str = this->l.eat();
            while (!this->l.peek(DOLLAR) && !this->l.peek(TAGSTART) && !this->l.peek(CLOSINGTAGSTART)) { // Merge all strings together
               str += " " + this->l.eat();
            }
            textnodes->push_back(new StringNode(str));
        }
    }
    return new ElementNode(textnodes);
}
