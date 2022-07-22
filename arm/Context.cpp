#include "Context.h"
void Context::init_ir_to_time(IR* ir){
    ir_to_time.insert({ir,++time});
}
