#ifndef HEADER_OPT
#define HEADER_OPT
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include "../IR/IR.hpp"
class OptimizerBuilder
{
private:
    vector<IR*> IRList;  //中间代码列表
public:
    void build(vector<IR*>& IRList);
    void MIRprint();
    void uniqueLable();//去除重复Label
    void Constant();//常量传播、折叠
};

#endif