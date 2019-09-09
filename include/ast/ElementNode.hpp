#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

#include "INode.h"
#include <vector>
#include <map>

class ElementNode : public INode
{
public:
    ElementNode(std::string elname, std::map<std::string, INode*> *attributes, std::vector<ElementNode*> *children)
        : elname(elname), attributes(attributes), children(children) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
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
protected:
    std::string elname;
    std::map<std::string, INode*> *attributes;
    std::vector<ElementNode*> *children;
};



class TextNode : public ElementNode
{
public:
    TextNode(std::string txt) : ElementNode("$" + txt, new std::map<std::string, INode*>(), new std::vector<ElementNode*>()) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "text( " << this->elname.substr(1) << " )";
        return ss.str();
    }
};

#endif /* ELEMENTNODE_HPP */
