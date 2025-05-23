#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <fstream>
#include <iomanip>


class ASTNode
{
public:
    virtual ~ASTNode() {}
    virtual void print() const = 0;
};

#endif