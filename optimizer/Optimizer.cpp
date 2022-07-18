#include "Optimizer.h"
void OptimizerBuilder::build(vector<IR*>& IRList)
{
    cout<<"开始优化"<<endl;
    this->IRList=IRList;
    uniqueLable();
    
    MIRprint();

}

void OptimizerBuilder::uniqueLable()
{
    
    vector<IR*> newList;
    set<string> s;
    for(auto ptr:IRList){
        if(ptr->op!=IR::_LABEL) newList.push_back(ptr);
        else {
            int oldsize=s.size();
            s.insert(ptr->opn1.name);
            if(s.size()!=oldsize)
                newList.push_back(ptr);
        }
    }
    IRList=newList;
}

void OptimizerBuilder::Constant()
{
    
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
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<" + "<<ir->opn2.name<<endl;
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
                    cout<<"\t"<<ir->result.name<<"= !"<<ir->opn1.name<<endl;
                    break;
            case IR::_POSI:       // result = + opn1
                    cout<<"\t"<<ir->result.name<<"= +"<<ir->opn1.name<<endl;
                    break;
            case IR::_NEGA:       // result = - opn1
                    cout<<"\t"<<ir->result.name<<"= -"<<ir->opn1.name<<endl;
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
                        cout<<"\t"<<"return "<<ir->opn1.name<<endl;
                    }
                    else{
                        cout<<"\t"<<"return "<<ir->opn1.name<<endl;
                    }
                    break;
            }
        }
    }

