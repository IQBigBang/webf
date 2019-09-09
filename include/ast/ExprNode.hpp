#ifndef EXPRNODE_HPP
#define EXPRNODE_HPP

#include "INode.h"
#include <vector>

class ExprNode : public INode
{
public:
    ExprNode(INode* func, std::vector<INode*> *args) : func(func), args(args) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
    {
        std::stringstream ss;
        ss << "func( f = " << this->func->repr() << ", ";
        int i = 0;
        for (INode* arg : *(this->args))
        {
            ss << "arg" << i << " = " << arg->repr() << ", ";
            i++;
        }
        ss << " )";
        return ss.str();
    }
private:
    INode* func;
    std::vector<INode*> *args;
};

#endif /* EXPRNODE_HPP */
