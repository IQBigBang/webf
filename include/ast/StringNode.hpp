#ifndef STRINGNODE_HPP
#define STRINGNODE_HPP

#include "INode.h"

class StringNode : public INode
{
public:
    StringNode(std::string s) : s(s) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "str(";
        ss << this->s;
        ss << ")";
        return ss.str();
    }
private:
    std::string s;
};

#endif /* STRINGNODE_HPP */
