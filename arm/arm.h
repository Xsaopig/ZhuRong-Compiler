#ifndef HEADER_ARM
#define HEADER_ARM
#include <vector>
#include <string>
#include <algorithm>
#include "../IR/IR.hpp"
using namespace std;


extern string filename;
void generate_arm(vector<IR*> IRList,std::ostream& out);
void generate_data_arm(vector<IR*> IRList,vector<IR*>::iterator data_begin,vector<IR*>::iterator data_end,std::ostream& out);
void generate_function_arm(vector<IR*> IRList,vector<IR*>::iterator function_begin,vector<IR*>::iterator function_end,std::ostream& out);

IR* find_opn_ASSIGN(vector<IR*>::iterator data_begin,vector<IR*>::iterator data_end,Opn& opn);
vector<int> PreCal_opn_int(vector<IR*>::iterator begin,vector<IR*>::iterator end,Opn& opn);//在begin和end之间提前计算opn的值
vector<float> PreCal_opn_float(vector<IR*>::iterator begin,vector<IR*>::iterator end,Opn& opn);//在begin和end之间提前计算opn的值
#endif