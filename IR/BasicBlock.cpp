#include "BasicBlock.h"
BasicBlock::BasicBlock(vector<IR*> IRList,int begin,int end)
{
    for(int i=begin;i<=end;i++){
        irlist.push_back(IRList[i]);
    }
}

vector<BasicBlock> buildBlocks(vector<IR*> IRList)
{
    vector<BasicBlock> blocks;
    auto iter=IRList.begin();
    int n=IRList.size();
    vector<int> flag(n,0);//1是开始，2是结束
    for(;iter!=IRList.end();iter++)
    {
        auto ir=*iter;
        int index=iter-IRList.begin();
        int target=-1;
        switch(ir->op){
            case IR::_FUNC:
                if(index-1>=0) flag[index-1]=2;
                flag[index]=1;
                break;
            case IR::_RET:
                flag[index]=2;
                if(index+1<n) flag[index+1]=1;
                break;
            case IR::_FUNC_END:
                flag[index]=2;
                if(index+1<n) flag[index+1]=1;
                break;
            case IR::_JEQ:
            case IR::_JGE:
            case IR::_JGT:
            case IR::_JLE:
            case IR::_JLT:
            case IR::_JNE:
                flag[index]=2;
                if(index+1<n) flag[index+1]=1;
                target=findLabel(IRList,ir->result);
                if(target!=-1)
                    flag[target]=1;
                break;
            case IR::_GOTO:
            case IR::_CALL:
                flag[index]=2;
                if(index+1<n) flag[index+1]=1;
                target=findFunc(IRList,ir->opn1);
                if(target!=-1)
                    flag[target]=1;
            break;
        }
    }
    int begin,end;
    bool begin_new=false;
    flag[0]=1;
    flag[n-1]=2;
    for(int i=0;i<n;i++){
        if(flag[i]==1){
            begin=i;
            begin_new=true;
        }
        else if(flag[i]==2){
            end=i;
            if(begin_new)
                blocks.push_back(BasicBlock(IRList,begin,end));
            else
                blocks.push_back(BasicBlock(IRList,end,end));
            begin_new=false;
        }
    }
    return blocks;
}

vector<IR*> blocksToIRList(vector<BasicBlock>& blocks)
{
    vector<IR*> irlist;
    for(auto &block:blocks)
        irlist.insert(irlist.end(),block.irlist.begin(),block.irlist.end());
    return irlist;
}

int findLabel(vector<IR*> IRList,Opn& target){
    for(int i=0;i<IRList.size();i++){
        auto& ir=IRList[i];
        if(ir->op==IR::_LABEL && ir->opn1==target)
            return i;
    }
    return -1;
}
int findFunc(vector<IR*> IRList,Opn& target){
    for(int i=0;i<IRList.size();i++){
        auto& ir=IRList[i];
        if(ir->op==IR::_FUNC && ir->opn1==target)
            return i;
    }
    return -1;
}
