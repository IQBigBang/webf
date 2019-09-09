#ifndef VARIABLENODE_HPP
#define VARIABLENODE_HPP

#include "INode.h"

class VariableNode : public INode
{
public:
    VariableNode( std::string varName) : varName(varName) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "var( " << this->varName << " )";
        return ss.str();
    }
private:
    std::string varName;
};

#endif /* VARIABLENODE_HPP */
