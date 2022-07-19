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


#endif