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
    virtual WebF_Type* execute(Runtime& r) = 0;
    virtual std::string repr() = 0;
};

class StringNode : public INode
{
public:
    StringNode(std::string s);
    WebF_Type* execute(Runtime& r);
    std::string repr();
private:
    std::string s;
};

class ExprNode : public INode
{
public:
    ExprNode(INode* func, std::vector<INode*> *args);
    WebF_Type* execute(Runtime& r);
    std::string repr();
private:
    INode* func;
    std::vector<INode*> *args;
};

class ListNode : public INode
{
public:
    ListNode(std::vector<INode*> *contents);
    WebF_Type* execute(Runtime& r);
    std::string repr();
private:
    std::vector<INode*> *contents;
};

class BlockNode : public INode
{
public:
    BlockNode(std::vector<ExprNode*> *contents);
    WebF_Type* execute(Runtime& r);
    std::string repr();
private:
    std::vector<ExprNode*> *contents;
};

class ElementNode : public INode
{
public:
    ElementNode(std::string elname, std::map<std::string, INode*> *attributes, std::vector<ElementNode*> *children);
    ElementNode(std::vector<INode*> *textnodes); // Text node
    WebF_Type* execute(Runtime& r);
    std::string repr();
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
    WebF_Type* execute(Runtime& r);
    std::string repr();
private:
    INode* object;
    std::string methodName;
};


#endif // AST_H
