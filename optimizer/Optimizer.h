#ifndef HEADER_OPT
#define HEADER_OPT
#include <vector>
#include <map>
#include <set>
#include <algorithm>
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
    void uniqueLable(vector<IR*>& irlist);//去除重复Label
    vector<IR*>& getIRList(){return this->IRList;}
    void Constant(vector<IR*>& irlist);//处理常量
    bool Constant_Folding(vector<IR*>& irlist);//常量折叠
    bool Constant_Propagation(vector<IR*>& irlist);//常量传播
};

#endif