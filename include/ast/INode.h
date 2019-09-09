#ifndef INODE_H
#define INODE_H

#include "WebF_Type.h"
#include "Runtime.h"
#include <string>
#include <sstream>

class INode
{
public:
    virtual WebF_Type* execute(Runtime& r) = 0;
    virtual std::string repr() = 0;
};

#endif /* INODE_H */
