#ifndef BLOCKNODE_HPP
#define BLOCKNODE_HPP

#include "INode.h"
#include "ExprNode.hpp"
#include <vector>

class BlockNode : public INode
{
public:
    BlockNode(std::vector<ExprNode*> *contents) : contents(contents) {}
    WebF_Type* execute(Runtime& r)
    {
        // TODO
    }
    std::string repr()
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
private:
    std::vector<ExprNode*> *contents;
};

#endif /* BLOCKNODE_HPP */
