#ifndef MEMBERACCESSNODE_HPP
#define MEMBERACCESSNODE_HPP

#include "INode.h"

class MemberAccessNode : public INode
{
public:
    MemberAccessNode(INode* object, std::string memberName) : object(object), memberName(memberName) {}
    IWebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "member( object = " << this->object->repr() << ", name = " << this->memberName << " )";
        return ss.str();
    }
private:
    INode* object;
    std::string memberName;
};

#endif /* MEMBERACCESSNODE_HPP */
