# H Programmer Language

Do you know when C file compile to binary it threw Assembly Language before compiling into binary?. So, this is my own CPU Emulator that inspired me from Intel x86 Architecture and Assembly Language.

H Programmer Language is the Emulator machine that running the H code to binary code.
This project has bug and the confusion of compiler and instruction program. *Dont Expect it Work Properly*. I will adjust it for next time.

# The Instruction set Format
the Instruction Set Format is the rule that make machine run functionlly

For this format:

| Prefix | Ext Prefix | Opcode | RegMem | Ext RegMem | Offset | Value |


- Prefix are require for instruction set, containing flags
- Ext Prefix are option based on PREFIX_EXT_INC on Prefix
- Opcode are require for instruction set, containing operation code like MOV, PUSH, POP, ADD, SUB, MUL and so on.
- RegMem are optional because it containing Register format:
    - 3 bits -> destination register
    - 2 bits -> reserved ( not implement )
    - 3 bits -> source register
- Ext Regmem is option based on RegMem.FLAGS=0b01
- Offset are optional but when PREFIX.M == 1, it turn to required
- Value are optional that containing value of data

# Inspire
- This project inspire with:
    - Intel x86 Documentation: [(Link Documentation)](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

# Conclusion
The H Programmer Language is based on Assembly Language but different scheme to run the code. This project should be imporved with collaboration with you to make this language is useful for future.

That's all from me, Thank you for review.