#ifndef AST_H
#define AST_H

#include "Runtime.h"
#include "WebF_Type.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>

class INode
{
public:
    virtual IWebF_Type* execute(Runtime& r) = 0;
    virtual std::string repr() = 0;
    virtual ~INode() {}
};

class StringNode : public INode
{
public:
    StringNode(std::string s);
    IWebF_Type* execute(Runtime& r);
    std::string repr();
private:
    std::string s;
};

class ExprNode : public INode
{
public:
    ExprNode(INode* func, std::vector<INode*> *args);
    IWebF_Type* execute(Runtime& r);
    std::string repr();
    virtual ~ExprNode() {delete this->func; delete this->args;}
private:
    INode* func;
    std::vector<INode*> *args;
};

class ListNode : public INode
{
public:
    ListNode(std::vector<INode*> *contents);
    IWebF_Type* execute(Runtime& r);
    std::string repr();
    virtual ~ListNode() {delete this->contents;}
private:
    std::vector<INode*> *contents;
};

class BlockNode : public INode
{
public:
    BlockNode(std::vector<ExprNode*> *contents);
    IWebF_Type* execute(Runtime& r);
    std::string repr();
    virtual ~BlockNode() {delete this->contents;}
private:
    std::vector<ExprNode*> *contents;
};

class ElementNode : public INode
{
public:
    ElementNode(std::string elname, std::map<std::string, INode*> *attributes, std::vector<ElementNode*> *children);
    ElementNode(std::vector<INode*> *textnodes); // Text node
    IWebF_Type* execute(Runtime& r);
    std::string repr();
    virtual ~ElementNode() {delete this->attributes; delete this->children; delete this->textnodes;}
private:
    std::string elname;
    std::map<std::string, INode*> *attributes;
    std::vector<ElementNode*> *children;
    std::vector<INode*> *textnodes;
};

class InstanceAccessNode : public INode
{
public:
    InstanceAccessNode(INode* object, std::string methodName);
    IWebF_Type* execute(Runtime& r);
    std::string repr();
    virtual ~InstanceAccessNode() {delete this->object;}
private:
    INode* object;
    std::string methodName;
};


#endif // AST_H
