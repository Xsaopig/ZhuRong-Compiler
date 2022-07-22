#include <map>
#include <string>
#include "../IR/IR.hpp"
using namespace std;
class Context {
private:
    int time = 0;
    int reg_count = 13;//R0~R12  ,编号为0~12
    int freg_count = 32;//S0~S31 ,编号为16~47
    vector<IR*> irs;
    vector<IR*>::iterator begin,end;
public:
    Context(vector<IR*>& IRList, vector<IR*>::iterator Begin,vector<IR*>::iterator End)
        :irs(IRList),begin(Begin),end(End) {};
    
    // 每一条IR对应的时间
    map<IR*, int> ir_to_time;
    void init_ir_to_time(IR* ir);
    
    //Var定义的时间
    map<string,int> var_define_to_time;
    map<int,string> time_to_var_define;

    //Var最后使用的时间
    map<string,int> var_lastused_to_time;
    map<int,string> time_to_var_lastused;

    // 当前在通用寄存器中的变量以及寄存器号
    map<string, int> var_to_reg;
    map<int, string> reg_to_var;




};