#include "Optimizer.h"

void OptimizerBuilder::build(vector<IR*>& IRList)
{
    cout<<"开始优化"<<endl;
    this->IRList=IRList;
    uniqueLable(this->IRList);
    Constant(this->IRList);
    MIRprint();

}

void OptimizerBuilder::Constant(vector<IR*>& irlist)
{
    bool change=false;
    do{
        if(Constant_Propagation(irlist)==true)
            change=true;
    }while(change==true);
}

bool OptimizerBuilder::Constant_Folding(vector<IR*>& irlist)//常量折叠
{
    for(auto ptr:irlist){
        if(ptr->op==IR::_ADD || ptr->op==IR::_SUB || ptr->op==IR::_MUL || ptr->op==IR::_DIV || ptr->op==IR::_MOD)
        {

        }
    }
    return true;
}
bool OptimizerBuilder::Constant_Propagation(vector<IR*>& irlist)//常量传播
{
    vector<IR*> deletelist;
    for(auto ptr:irlist){
        if(ptr->op!=IR::_ASSIGN && ptr->op!=IR::_Arr_ASSIGN && ptr->op!=IR::_NOT && ptr->op!=IR::_POSI && ptr->op!=IR::_NEGA)
            continue;
        else if(ptr->opn1.kind!=Opn::Imm)
            continue;
        else{
            if(ptr->op==IR::_NOT)
            {
                ptr->op=IR::_ASSIGN;
                ptr->opn1.imm_float=-(ptr->opn1.imm_float==0)?1:0;
                ptr->opn1.imm_int=(ptr->opn1.imm_int==0)?1:0;
            }
            else if(ptr->op==IR::_POSI)
            {
                ptr->op=IR::_ASSIGN;
                ptr->opn1.imm_float=+(ptr->opn1.imm_float);
                ptr->opn1.imm_int=+(ptr->opn1.imm_int);
            }
            else if(ptr->op==IR::_NEGA)
            {
                ptr->op=IR::_ASSIGN;
                ptr->opn1.imm_float=-(ptr->opn1.imm_float);
                ptr->opn1.imm_int=-(ptr->opn1.imm_int);
            }
            for(auto ptr1:irlist){
                if(ptr1->op==IR::_ALLOC || ptr1->op==IR::_LABEL || ptr1->op==IR::_VOID) continue;
                if(ptr1->opn1.kind==Opn::Var && ptr1->opn1.name.compare(ptr->result.name)==0 )
                    ptr1->opn1=ptr->opn1;
                if(ptr1->opn2.kind==Opn::Var && ptr1->opn2.name.compare(ptr->result.name)==0 )
                    ptr1->opn2=ptr->opn1;
            }
            deletelist.push_back(ptr);
        }
    }
    for(auto ptr:deletelist)
    {
        auto it=find(irlist.begin(),irlist.end(),ptr);
        it=irlist.erase(it);
    }
    if(deletelist.size()>0)
        return true;
    else return false;
}

void OptimizerBuilder::uniqueLable(vector<IR*>& irlist)
{
    vector<IR*> newList;
    set<string> s;
    for(auto ptr:irlist){
        if(ptr->op!=IR::_LABEL) newList.push_back(ptr);
        else {
            int oldsize=s.size();
            s.insert(ptr->opn1.name);
            if(s.size()!=oldsize)
                newList.push_back(ptr);
        }
    }
    irlist=newList;
}

void OptimizerBuilder::MIRprint()
    {
        IR *ir;
        Opn *opn;
        int i;
        for (i = 0; i < IRList.size(); i++)
        {
            ir = IRList[i];
            switch (ir->op)
            {
            case IR::_VOID:       // 无用指令
                    break;
            case IR::_FUNC:      // opn1 :
                    cout<<"define function "<<ir->opn1.name<<endl;   
            case IR::_LABEL:      // opn1 :
                    cout<<ir->opn1.name<<": "<<endl;
                    break;
            case IR::_ALLOC:      // alloc opn1(变量名) : result(字节数)
                    cout<<"\talloc "<<ir->opn1.name<<": "<<ir->result.imm_int<<endl;
                    break;
            case IR::_ADD:        // result = opn1 + opn2
                    cout<<"\t"<<ir->result.name<<" = ";
                    if(ir->opn1.kind==Opn::Var)
                        cout<<ir->opn1.name;
                    else if(ir->opn1.kind==Opn::Imm)
                        ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                    cout<<" + ";
                    if(ir->opn2.kind==Opn::Var)
                        cout<<ir->opn2.name;
                    else if(ir->opn2.kind==Opn::Imm)
                        ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                    cout<<endl;
                    break;
            case IR::_SUB:        // result = opn1 - opn2
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<" - "<<ir->opn2.name<<endl;
                    break;
            case IR::_MUL:        // result = opn1 * opn2
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<" * "<<ir->opn2.name<<endl; 
                    break;           
            case IR::_DIV:        // result = opn1 / opn2
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<" / "<<ir->opn2.name<<endl;
                    break; 
            case IR::_MOD:        // result = opn1 % opn2
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<" % "<<ir->opn2.name<<endl;
                    break; 
            case IR::_ASSIGN:     // result = opn1
                    if(ir->opn1.kind==Opn::Imm){
                        if(ir->opn1.is_int==1){
                            cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.imm_int<<endl;;
                        }
                        else{
                            cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.imm_float<<endl;
                        }
                    }
                    else if(ir->opn1.kind==Opn::Var){
                            cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<endl;
                    }
                    else if(ir->opn1.kind==Opn::Block){
                            cout<<"\t"<<ir->result.name<<" ={ ";
                            for(int i=0;i<ir->opn1.Block_args.size();i++){
                                opn=ir->opn1.Block_args[i];
                                cout<<opn->name<<" ";
                            }  
                            cout<<"}"<<endl;                    
                    }
                    break;
            case IR::_Arr_ASSIGN: // opn1[opn2]=result
                    cout<<"\t"<<ir->opn1.name<<"["<<ir->opn2.name<<"]"<<" = "<<ir->result.name<<endl;
                    break;
            case IR::_ASSIGN_Arr: // op1=opn2[result]
                    cout<<"\t"<<ir->opn1.name<<" = "<<ir->opn2.name<<"["<<ir->result.name<<"]"<<endl;
                    break;
            case IR::_NOT:        // result = ! opn1
                    if(ir->opn1.kind==Opn::Var)
                        cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.name<<endl;
                    else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.imm_int<<endl;
                    else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.imm_float<<endl;
                    break;
            case IR::_POSI:       // result = + opn1
                    if(ir->opn1.kind==Opn::Var)
                        cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.name<<endl;
                    else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.imm_int<<endl;
                    else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.imm_float<<endl;
                    break;
            case IR::_NEGA:       // result = - opn1
                    if(ir->opn1.kind==Opn::Var)
                        cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.name<<endl;
                    else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.imm_int<<endl;
                    else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                        cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.imm_float<<endl;
                    break;
            case IR::_GOTO:       // goto opn1
                    cout<<"\t"<<"goto "<<ir->opn1.name<<endl;
                    break;           
            case IR::_JEQ:        // if opn1 == opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" == "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_JNE:        // if opn1 != opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" != "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_JLT:        // if opn1 < opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" < "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_JGT:        // if opn1 > opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" > "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_JLE:        // if opn1 <= opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" <= "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_JGE:        // if opn1 >= opn2 goto result
                    cout<<"\t"<<"if "<<ir->opn1.name<<" >= "<<ir->opn2.name<<" goto "<<ir->result.name<<endl;
                    break;
            case IR::_PARAM:      // param opn1
                    cout<<"\t"<<"param "<<ir->opn1.name<<endl;
                    break;
            case IR::_CALL:       // [result =] call opn1(函数) , opn2(参数个数)
                    if(ir->result.kind==Opn::Null){
                        cout<<"\t"<<"call "<<ir->opn1.name<<", "<<ir->opn2.imm_int<<endl;
                    }
                    else{
                        cout<<"\t"<<ir->result.name<<" =call "<<ir->opn1.name<<", "<<ir->opn2.imm_int<<endl;
                    }
                    break;
            case IR::_RET:        // return [opn1]
                    if(ir->opn1.kind==Opn::Null){
                        cout<<"\t"<<"return "<<endl;
                    }
                    else if(ir->opn1.kind==Opn::Var){
                        cout<<"\t"<<"return "<<ir->opn1.name<<endl;
                    }
                    else if(ir->opn1.kind==Opn::Imm){
                        if(ir->opn1.is_int)
                        cout<<"\t"<<"return "<<ir->opn1.imm_int<<endl;
                        else
                        cout<<"\t"<<"return "<<ir->opn1.imm_float<<endl;
                    }
                    break;
            }
        }
    }

