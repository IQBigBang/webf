#ifndef LISTNODE_HPP
#define LISTNODE_HPP

#include "INode.h"
#include <vector>

class ListNode : public INode
{
public:
    ListNode(std::vector<INode*> *contents) : contents(contents) {}
    IWebF_Type* execute(Runtime& r)
    {

    }
    std::string repr()
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
private:
    std::vector<INode*> *contents;
};

#endif /* LISTNODE_HPP */
