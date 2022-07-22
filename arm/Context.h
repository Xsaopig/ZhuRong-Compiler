#include <map>
#include <string>
#include <bitset>
#include "../IR/IR.hpp"
using namespace std;
class Context {
private:
    int time = 0;
    vector<IR*> irs;
    vector<IR*>::iterator begin,end;
public:
    // static constexpr int freg_count = 32;//S0~S31 ,编号为16~47
    static constexpr int reg_count = 12;//R0~R12  ,编号为0~12   ,R13只负责保存栈顶地址

    Context(vector<IR*>& IRList, vector<IR*>::iterator Begin,vector<IR*>::iterator End)
        :irs(IRList),begin(Begin),end(End) {};
    
    // 每一条IR对应的时间
    map<IR*, int> ir_to_time;
    void init_ir_to_time(IR* ir);
    
    //Var定义的时间
    map<string,int> var_define_to_time;
    multimap<int,string> time_to_var_define;
    void init_var_define_to_time(IR* ir);

    //Var最后使用的时间
    map<string,int> var_lastused_to_time;
    multimap<int,string> time_to_var_lastused;
    void init_var_lastused_to_time(IR* ir);
    void update_lastused_time(int time,Opn& opn);

    //栈已经占用字节数
    // 0 : 暂未使用
    // 1 : 保护现场
    // 2 : 寄存器溢出
    // 3 : 函数调用
    int stack_occupied[4]={0, 4, 0, 0};

    //分配在栈中的变量名，以及它的偏移量
    map<string,int> stack_offset;
    

    //已使用的通用寄存器
    bitset<reg_count>  used_reg=0b000000000000;

    // 当前在通用寄存器中的变量以及寄存器号
    map<string, int> var_to_reg;
    map<int, string> reg_to_var;

    //查询变量是否在寄存器中
    bool var_in_reg(string name) {return var_to_reg.count(name);}

    //查询可用寄存器，没有就返回-1
    int find_free_reg(int begin);

    //得到可用寄存器，没有就选择一个寄存器溢出，将其中变量存放到栈中
    int get_new_reg(int begin);

    //溢出寄存器
    void overflow_reg(int id);

    //溢出变量，将变量存到栈中,变量在栈中占用bytes个字节
    void overflow_var(string name,int bytes);

    //清楚非活跃的区间
    void clear_inactive_intervals(int cur_time);

    
    
    // 加载立即数到寄存器中
    void load_imm_int(string reg, int value, ostream& out);

    // void load_float(string reg, int value, ostream& out);

    void load(string reg, Opn& op, ostream& out);

    void store_to_stack_offset(string reg, int offset, ostream& out,string op = "STR");

    void load_from_stack_offset(string reg, int offset, ostream& out,string op = "LDR");

    void store_to_stack(string reg, Opn& op, ostream& out,string op_code = "STR");



};