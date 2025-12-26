#include "Register.hpp"
#include <cstdio>
#include <assert.h>

vector<Register*> Register::registerStack;

Register* Register::getAllocRegister() {
    for(Register& reg : registerList )
        if( !reg.isUse )
        {
            reg.isUse = true;
#ifdef __DEBUG__
            printf("[Register] Alloc new Register %s\n", reg.name.c_str());
#endif
            return &reg;
        }

    return nullptr;
}

void Register::pushRegister( Register* reg ) {
#ifdef __DEBUG__ 
    printf("[Register] push Register '%s'\n", reg->name.c_str());
#endif
    Register::registerStack.push_back( reg );
}

void Register::UnusedAllRegister() {
    for(Register* reg : Register::registerStack)
    {
        reg->isUse = false;
    }
    Register::registerStack.clear();
}

Register* Register::popRegister() {
    assert( registerStack.size() != 0 );
    Register* reg = Register::registerStack[ registerStack.size() - 1 ];
#ifdef __DEBUG__
    printf("[Register] pop Register '%s'\n",reg->name.c_str());
#endif
    Register::registerStack.pop_back();
    return reg;
}

void Register::UnusedRegister(Register *reg)
{
#ifdef __DEBUG__ 
    printf("[Register] Unused Register %s\n", reg->name.c_str());
#endif
    reg->isUse = false;
}
