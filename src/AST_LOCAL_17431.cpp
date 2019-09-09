#include "AST.h"

StringNode::StringNode(std::string s) : s(s) {}
IWebF_Type* StringNode::execute(Runtime& r)
{
    // TODO
}
std::string StringNode::repr()
{
    std::stringstream ss;
    ss << "str(";
    ss << this->s;
    ss << ")";
    return ss.str();
}

ExprNode::ExprNode(INode* func, std::vector<INode*> *args) : func(func), args(args) {}
IWebF_Type* ExprNode::execute(Runtime& r)
{
    // TODO
}
std::string ExprNode::repr()
{
    std::stringstream ss;
    ss << "func( name = " << this->func->repr() << ", ";
    int i = 0;
    for (INode* arg : *(this->args))
    {
        ss << "arg" << i << " = " << arg->repr() << ", ";
        i++;
    }
    ss << " )";
    return ss.str();
}

ListNode::ListNode(std::vector<INode*> *contents) : contents(contents) {}
IWebF_Type* ListNode::execute(Runtime& r)
{
    // TODO
}
std::string ListNode::repr()
{
    std::stringstream ss;
    ss << "list( ";
    int i = 0;
    for (INode* n : *(this->contents))
    {
        ss << i << " = " << n->repr() << ", ";
        i++;
    }
    ss << " )";
    return ss.str();
}

BlockNode::BlockNode(std::vector<ExprNode*> *contents) : contents(contents) {}
IWebF_Type* BlockNode::execute(Runtime& r)
{
    // TODO
}
std::string BlockNode::repr()
{
    std::stringstream ss;
    ss << "block( ";
    for (INode* n : *(this->contents))
    {
        ss << n->repr() << ", ";
    }
    ss << " )";
    return ss.str();
}

ElementNode::ElementNode(std::string elname,
                        std::map<std::string, INode*> *attributes,
                        std::vector<ElementNode*> *children)
    : elname(elname), attributes(attributes), children(children)
{}
ElementNode::ElementNode(std::vector<INode*> *textnodes) : textnodes(textnodes)
{
    this->elname = "$txt"; // Syntactically impossible element name
}
IWebF_Type* ElementNode::execute(Runtime& r)
{
    // TODO
}
std::string ElementNode::repr()
{
    if (this->elname == "$txt")
    {
        std::stringstream ss;
        ss << "text( ";
        for (INode* n : *(this->textnodes))
        {
            ss << n->repr() << ", ";
        }
        ss << " )";
        return ss.str();
    }
    std::stringstream ss;
    ss << "element( elname = " << this->elname << ", args = ( ";
    for (auto const& a : *(this->attributes))
    {
        ss << a.first << " = " << a.second->repr() << "; ";
    }
    ss << " ), children = (";
    for (ElementNode* n: *(this->children))
    {
        ss << n->repr();
        ss << ", ";
    }
    ss << " ))";
    return ss.str();
}

InstanceAccessNode::InstanceAccessNode(INode* object, std::string methodName) : object(object), methodName(methodName) {}
IWebF_Type* InstanceAccessNode::execute(Runtime& r) {}
std::string InstanceAccessNode::repr()
{
    std::stringstream ss;
    ss << "method( object = " << this->object->repr() << ", name = " << this->methodName << " )";
    return ss.str();
}
