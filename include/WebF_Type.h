#ifndef IWEBF_TYPE_H
#define IWEBF_TYPE_H

#include <string>
#include <vector>

class IWebF_Type
{
public:
    // Interfaces:  iChainable (has '++', 'chain' method)
    //              iAlgebraic (has '+', 'add', '-', 'sub', '*', 'mul', '/', 'div' methods)
    //              iIndexable (has 'at', 'len' method)
    //              iCastable<type> (is castable to type)
    virtual bool implementsInterface(std::string iName) = 0;
    virtual std::string getType() = 0;
    virtual IWebF_Type* invokeMethod(std::string methodName, std::vector<IWebF_Type*>* args) = 0;
};

#endif // IWEBF_TYPE_H
