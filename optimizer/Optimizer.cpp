#include "Optimizer.h"

void OptimizerBuilder::build(vector<IR*>& IRList)
{
    cout<<"开始优化"<<endl;
    this->IRList=IRList;
    uniqueLable(this->IRList);
    auto blocks=buildBlocks(this->IRList);
    for(auto& block:blocks)
    {
        Constant(block.irlist);
    }
    this->IRList=blocksToIRList(blocks);
    // Constant(this->IRList);
    while(Del_DeadCode(this->IRList));
    MIRprint();

}

bool OptimizerBuilder::Del_DeadCode(vector<IR*>& irlist)
{
    bool res=false;
    for(int i=0;i<irlist.size();i++){
        auto ptr=irlist[i];
        auto op=ptr->op;
        if(op!=IR::_ASSIGN && op!=IR::_NOT && op!=IR::_POSI && op!=IR::_NEGA  
            && op!=IR::_ADD && op!=IR::_SUB && op!=IR::_MUL && op!=IR::_DIV && op!=IR::_MOD
            && op!=IR::_ASSIGN_Arr && op!=IR::_ADDR && op!=IR::_ALLOC)
            continue;
        else{
            bool used=false;
            Opn* opn=&ptr->result;
            if(op==IR::_ALLOC)
                opn=&ptr->opn1;
            for(int j=i+1;j<irlist.size();j++){
                auto ptr1=irlist[j];
                if(ptr1->opn1.kind==Opn::Block){
                    for(auto& block_opn:ptr1->opn1.Block_args){
                        if(*opn==*block_opn){
                            used=true;
                            break;
                        }
                    }
                }
                else if(ptr1->opn1==*opn || ptr1->opn2==*opn || ptr1->result==*opn) {
                    used=true;
                    break;
                }
            }
            if(used==false){
                irlist.erase(irlist.begin()+i);
                i--;
                res=true;
            }
        }
    }
    return res;
}

bool OptimizerBuilder::Constant(vector<IR*>& irlist)
{
    bool neverchange=true;
    bool change=false;
    do{
        change=false;
        if(Constant_Propagation(irlist)==true){
            change=true;
            neverchange=false;
        }
        if(Constant_Folding(irlist)==true){
            change=true;
            neverchange=false;
        }
    }while(change==true);
    return neverchange;
}

bool OptimizerBuilder::Constant_Folding(vector<IR*>& irlist)//常量折叠
{
    bool res=false;
    for(auto ptr:irlist){
        switch (ptr->op)
        {
        case IR::_NOT:
            if(ptr->opn1.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            ptr->opn1.imm_int=(ptr->opn1.imm_int==0)?1:0;
            ptr->opn1.imm_float=(ptr->opn1.imm_int==0)?1:0;
            break;
        case IR::_POSI:
            if(ptr->opn1.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            ptr->opn1.imm_int=(+ptr->opn1.imm_int);
            ptr->opn1.imm_float=(+ptr->opn1.imm_int);
            break;
        case IR::_NEGA:
            if(ptr->opn1.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            ptr->opn1.imm_int=(-ptr->opn1.imm_int);
            ptr->opn1.imm_float=(-ptr->opn1.imm_int);
            break;
        case IR::_ADD:
            if(ptr->opn1.kind!=Opn::Imm || ptr->opn2.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            if(ptr->opn1.is_int==ptr->opn2.is_int){
                ptr->opn1.imm_int+=ptr->opn2.imm_int;
                ptr->opn1.imm_float+=ptr->opn2.imm_float;
            }
            else{
                if(ptr->opn1.is_int){
                    ptr->opn1.imm_float=ptr->opn1.imm_int+ptr->opn2.imm_float;
                    ptr->opn1.is_int=false;
                }else{
                    ptr->opn1.imm_float=ptr->opn1.imm_float+ptr->opn2.imm_int;
                    ptr->opn1.is_int=false;
                }
            }
            ptr->opn2.kind=Opn::Null;
            break;
        case IR::_SUB:
            if(ptr->opn1.kind!=Opn::Imm || ptr->opn2.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            if(ptr->opn1.is_int==ptr->opn2.is_int){
                ptr->opn1.imm_int-=ptr->opn2.imm_int;
                ptr->opn1.imm_float-=ptr->opn2.imm_float;
            }
            else{
                if(ptr->opn1.is_int){
                    ptr->opn1.imm_float=ptr->opn1.imm_int-ptr->opn2.imm_float;
                    ptr->opn1.is_int=false;
                }else{
                    ptr->opn1.imm_float=ptr->opn1.imm_float-ptr->opn2.imm_int;
                    ptr->opn1.is_int=false;
                }
            }
            ptr->opn2.kind=Opn::Null;
            break;
        case IR::_MUL:
            if(ptr->opn1.kind!=Opn::Imm || ptr->opn2.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            if(ptr->opn1.is_int==ptr->opn2.is_int){
                ptr->opn1.imm_int*=ptr->opn2.imm_int;
                ptr->opn1.imm_float*=ptr->opn2.imm_float;
            }
            else{
                if(ptr->opn1.is_int){
                    ptr->opn1.imm_float=ptr->opn1.imm_int*ptr->opn2.imm_float;
                    ptr->opn1.is_int=false;
                }else{
                    ptr->opn1.imm_float=ptr->opn1.imm_float*ptr->opn2.imm_int;
                    ptr->opn1.is_int=false;
                }
            }
            ptr->opn2.kind=Opn::Null;
            break;
        case IR::_DIV:
            if(ptr->opn1.kind!=Opn::Imm || ptr->opn2.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;
            if(ptr->opn1.is_int==ptr->opn2.is_int){
                ptr->opn1.imm_int/=ptr->opn2.imm_int;
                ptr->opn1.imm_float/=ptr->opn2.imm_float;
            }
            else{
                if(ptr->opn1.is_int){
                    ptr->opn1.imm_float=ptr->opn1.imm_int/ptr->opn2.imm_float;
                    ptr->opn1.is_int=false;
                }else{
                    ptr->opn1.imm_float=ptr->opn1.imm_float/ptr->opn2.imm_int;
                    ptr->opn1.is_int=false;
                }
            }
            ptr->opn2.kind=Opn::Null;
            break;
        case IR::_MOD:
            if(ptr->opn1.kind!=Opn::Imm || ptr->opn2.kind!=Opn::Imm) break;
            res=true;
            ptr->op=IR::_ASSIGN;//C语言规定取余操作只能作用于int类型
            ptr->opn1.imm_int%=ptr->opn2.imm_int;
            ptr->opn2.kind=Opn::Null;
            break;
        default:
            break;
        }
    }
    ASSIGN_type_conversion(irlist);
    return res;
}
bool OptimizerBuilder::Constant_Propagation(vector<IR*>& irlist)//常量传播
{
    bool res=false;
    for(int i=0;i<irlist.size();i++){
        auto ptr=irlist[i];
        if(ptr->op!=IR::_ASSIGN && ptr->op!=IR::_Arr_ASSIGN && ptr->op!=IR::_ASSIGN_Arr )
            continue;
        else if(ptr->opn1.kind!=Opn::Imm)
            continue;
        else{
            bool exit=false;
            for(int j=i+1;j<irlist.size();j++){//查找下一条使用了操作数ptr->result语句来进行替换
                auto ptr1=irlist[j];
                if(exit) break;
                else if(ptr1->op==IR::_VOID || ptr1->op==IR::_ADDR) continue;
                else if(ptr1->op==IR::_JEQ || ptr1->op==IR::_JGE || ptr1->op==IR::_JGT || ptr1->op==IR::_JLE 
                || ptr1->op==IR::_JLT || ptr1->op==IR::_JNE) break;
                else if(ptr1->result==ptr->result || ptr1->op==IR::_GOTO || 
                ptr1->op==IR::_FUNC || ptr1->op==IR::_CALL || ptr1->op==IR::_FUNC_END 
                || ptr1->op==IR::_RET) exit=true;
                if(ptr1->op!=IR::_ASSIGN_Arr && ptr1->op!=IR::_Arr_ASSIGN && ptr1->op!=IR::_ALLOC)
                {
                    if(ptr1->opn1.kind==Opn::Var && ptr1->opn1.name.compare(ptr->result.name)==0 ){
                        if(ptr->opn1.is_int==ptr1->opn1.is_int)
                            ptr1->opn1=ptr->opn1;
                        else{//替换双方类型不同，需要转换
                            ptr1->opn1=ptr->opn1;
                            ptr1->opn1.int_float_convert();
                        }
                        res=true;
                    }
                    else if(ptr1->opn1.kind==Opn::Block)
                    {
                        for(int i=0;i<ptr1->opn1.Block_args.size();i++){
                            if(ptr1->opn1.Block_args[i]->kind==Opn::Var && ptr1->opn1.Block_args[i]->name.compare(ptr->result.name)==0){
                                if(ptr->opn1.is_int==ptr1->opn1.Block_args[i]->is_int)
                                    *ptr1->opn1.Block_args[i]=ptr->opn1;
                                else{//替换双方类型不同，需要转换
                                    *ptr1->opn1.Block_args[i]=ptr->opn1;
                                    ptr1->opn1.Block_args[i]->int_float_convert();
                                }
                            }
                        }
                    }
                    if(ptr1->opn2.kind==Opn::Var && ptr1->opn2.name.compare(ptr->result.name)==0 ){
                        if(ptr->opn1.is_int==ptr1->opn2.is_int)
                            ptr1->opn2=ptr->opn1;
                        else{//替换双方类型不同，需要转换
                            ptr1->opn2=ptr->opn1;
                            ptr1->opn2.int_float_convert();
                        }
                        res=true;
                    }
                }
                else if(ptr1->op==IR::_ALLOC){
                    if(ptr1->result.kind==Opn::Var && ptr1->result.name.compare(ptr->result.name)==0){
                        ptr1->result=ptr->opn1;
                        res=true;
                    }
                }
                else{
                    if(ptr1->opn2.kind==Opn::Var && ptr1->opn2.name.compare(ptr->result.name)==0 ){
                        ptr1->opn2=ptr->opn1;
                        res=true;
                    }
                    if(ptr1->op==IR::_ASSIGN_Arr) continue;
                    if(ptr1->result.kind==Opn::Var && ptr1->result.name.compare(ptr->result.name)==0 ){
                        ptr1->result=ptr->opn1;
                        res=true;
                    }
                }
            }
        }
    }
    return res;
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

void OptimizerBuilder::ASSIGN_type_conversion(vector<IR*>& irlist)
{
    for(auto ptr:irlist){
        if(ptr->op==IR::_ASSIGN){
            if(ptr->opn1.is_int==ptr->result.is_int) continue;
            if(ptr->opn1.is_int)
                ptr->opn1.imm_float=ptr->opn1.imm_int;
            else
                ptr->opn1.imm_int=ptr->opn1.imm_float;
            ptr->opn1.is_int=!ptr->opn1.is_int;
        }
    }
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
                if(ir->result.kind==Opn::Imm)
                    cout<<"\talloc "<<ir->opn1.name<<": "<<ir->result.imm_int<<endl;
                else
                    cout<<"\talloc "<<ir->opn1.name<<": "<<ir->result.name<<endl;
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
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" - ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break;
        case IR::_MUL:        // result = opn1 * opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" * ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break;           
        case IR::_DIV:        // result = opn1 / opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" / ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break; 
        case IR::_MOD:        // result = opn1 % opn2
                cout<<"\t"<<ir->result.name<<" = ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm)
                    ir->opn1.is_int?(cout<<ir->opn1.imm_int):(cout<<ir->opn1.imm_float);
                cout<<" % ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    ir->opn2.is_int?(cout<<ir->opn2.imm_int):(cout<<ir->opn2.imm_float);
                cout<<endl;
                break; 
        case IR::_ADDR:
                cout<<"\t"<<ir->result.name<<" = &"<<ir->opn1.name<<endl;
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
                else if(ir->opn1.kind==Opn::Var  || ir->opn1.kind==Opn::Array){
                        cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<endl;
                }
                else if(ir->opn1.kind==Opn::Block){
                        cout<<"\t"<<ir->result.name<<" ={ ";
                        for(int i=0;i<ir->opn1.Block_args.size();i++){
                            opn=ir->opn1.Block_args[i];
                            if(opn->kind==Opn::Var)
                                cout<<opn->name<<" ";
                            else if(opn->kind==Opn::Imm){
                                if(opn->is_int) cout<<opn->imm_int<<" ";
                                else cout<<opn->imm_float<<" ";
                            }
                        }  
                        cout<<"}"<<endl;                    
                }
                break;
        case IR::_Arr_ASSIGN: // opn1[opn2]=result

                cout<<"\t"<<ir->opn1.name<<"[";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm)
                    if(ir->opn2.is_int) cout<<ir->opn2.imm_int;
                    else cout<<ir->opn2.imm_float;
                cout<<"] = ";
                if(ir->result.kind==Opn::Var)
                    cout<<ir->result.name;
                else if(ir->result.kind==Opn::Imm)
                    if(ir->result.is_int) cout<<ir->result.imm_int;
                    else cout<<ir->result.imm_float;
                cout<<endl;
                break;
        case IR::_ASSIGN_Arr: // result=opn1[opn2]
                if(ir->opn2.kind==Opn::Var)
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<"["<<ir->opn2.name<<"]"<<endl;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<"["<<ir->opn2.imm_int<<"]"<<endl;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<"\t"<<ir->result.name<<" = "<<ir->opn1.name<<"["<<ir->opn2.imm_float<<"]"<<endl;
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
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" == ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JNE:        // if opn1 != opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" != ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JLT:        // if opn1 < opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" < ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JGT:        // if opn1 > opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" > ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JLE:        // if opn1 <= opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" <= ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_JGE:        // if opn1 >= opn2 goto result
                cout<<"\t"<<"if ";
                if(ir->opn1.kind==Opn::Var)
                    cout<<ir->opn1.name;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<ir->opn1.imm_int;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<ir->opn1.imm_float;
                cout<<" >= ";
                if(ir->opn2.kind==Opn::Var)
                    cout<<ir->opn2.name;
                else if(ir->opn2.kind==Opn::Imm && ir->opn2.is_int)
                    cout<<ir->opn2.imm_int;
                else if(ir->opn2.kind==Opn::Imm && !ir->opn2.is_int)
                    cout<<ir->opn2.imm_float;
                cout<<" goto "<<ir->result.name<<endl;
                break;
        case IR::_PARAM:      // param opn1
                if(ir->opn1.kind==Opn::Var)
                    cout<<"\t"<<"param "<<ir->opn1.name<<endl;
                else if(ir->opn1.kind==Opn::Imm && ir->opn1.is_int)
                    cout<<"\t"<<"param "<<ir->opn1.imm_int<<endl;
                else if(ir->opn1.kind==Opn::Imm && !ir->opn1.is_int)
                    cout<<"\t"<<"param "<<ir->opn1.imm_float<<endl;
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

