#ifndef NUMBERNODE_HPP
#define NUMBERNODE_HPP

#include "INode.h"

class NumberNode : public INode
{
public:
    NumberNode(double d) : d(d) {}
    WebF_Type* execute(Runtime& r)
    {

    }
    std::string repr()
    {
            std::stringstream ss;
        ss << "num(";
        ss << this->d;
        ss << ")";
        return ss.str();
    }
private:
    double d;
};

#endif /* NUMBERNODE_HPP */
