#ifndef VARDEFNODE_HPP
#define VARDEFNODE_HPP

#include "INode.h"

class VardefNode : public INode
{
public:
    VardefNode( std::string varName) : varName(varName) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "vardef( " << this->varName << " )";
        return ss.str();
    }
private:
    std::string varName;
};

#endif /* VARDEFNODE_HPP */
