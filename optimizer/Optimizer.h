#ifndef HEADER_OPT
#define HEADER_OPT
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include "../IR/IR.hpp"
#include "../IR/BasicBlock.h"
class OptimizerBuilder
{
private:
    vector<IR*> IRList;  //中间代码列表
public:
    void build(vector<IR*>& IRList);
    void MIRprint();
    void uniqueLable(vector<IR*>& irlist);//去除重复Label
    vector<IR*>& getIRList(){return this->IRList;}
    bool Constant(vector<IR*>& irlist);//处理常量
    bool Constant_Folding(vector<IR*>& irlist);//常量折叠
    bool Constant_Propagation(vector<IR*>& irlist);//常量传播

    bool Del_DeadCode(vector<IR*>& irlist);//死代码删除

    void ASSIGN_type_conversion(vector<IR*>& irlist);//将所有赋值语句的右边操作数类型保证与左边对齐


};

#endif