#include <vector>
#include "../IR/IR.hpp"
typedef struct BasicBlock
{
public:
    vector<IR*> irlist;
    BasicBlock *l,*r;
    BasicBlock(vector<IR*> IRList,int begin,int end);
} BasicBlock;


vector<BasicBlock> buildBlocks(vector<IR*> IRList);
vector<IR*> blocksToIRList(vector<BasicBlock>& blocks);
int findLabel(vector<IR*> IRList,Opn& target);
int findFunc(vector<IR*> IRList,Opn& target);