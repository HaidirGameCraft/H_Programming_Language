#pragma once

#include <string>
#include <vector>

using namespace std;

class Register {
public:
    string name;
    bool isUse = false;

    // The Stack of register
    static vector<Register*> registerStack;
    Register() {};
    Register(string name) : name(name) {}

    // Getting the available Register
    static Register* getAllocRegister();

    // Pushing the register into stack Register for future use
    static void pushRegister( Register* reg );

    // Popping get the previous Register from stack
    static Register* popRegister();

    // Clear and Set all Register as Unused Register
    static void UnusedAllRegister();

    // Clear and Set the specific Register as Unused Register
    static void UnusedRegister( Register* reg );
};

/// @brief Declare and Create CPU Register
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
