#pragma once

#include <string>
#include <vector>

using namespace std;

class Register {
public:
    string name;
    bool isUse = false;
    static vector<Register*> registerStack;
    Register() {};
    Register(string name) : name(name) {}

    static Register* getAllocRegister();
    static void pushRegister( Register* reg );
    static Register* popRegister();
    static void UnusedAllRegister();
    static void UnusedRegister( Register* reg );
};

static Register registerList[] = {
    { "ra" },
    { "rb" },
    { "rc" },
    { "rd" },
    { "re" },
    { "rf" },
    { "rg" },
    { "rh" },
    { "ri" },
    { "rj" },
    { "rk" },
    { "rl" },
    { "rm" },
    { "rn" },
    { "rs" },
    { "rp" },
};
