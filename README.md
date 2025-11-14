# H Programmer Langguage

Do you know when C file compile to binary it threw Assembly Language before compiling into binary?. So, this is my own CPU Emulator that inspired me from Intel x86 Architecture and Assembly Langguage.

H Programmer Language is the Emulator machine that running the H code to binary code. 

# The Instruction set Format
the Instruction Set Format is the rule that make machine run functionlly

For this format:

| Prefix | Opcode | RegMem | Offset | Value |


- Prefix are require for instruction set, containing flags
- Opcode are require for instruction set, containing operation code like MOV, PUSH, POP, ADD, SUB, MUL and so on.
- RegMem are optional because it containing Register format:
    - 3 bits -> destination register
    - 2 bits -> reserved ( not implement )
    - 3 bits -> source register
- RegMem can be use as Memory while the PREFIX.M is == 1, allow the offset are used in this case
- Offset are optional but when PREFIX.M == 1, it turn to required
- Value are optional that containing value of data

# Inspire
- This project inspire with:
    - Intel x86 Documentation

# Conclusion
The H Programmer Language is based on Assembly Language but different scheme to run the code. This project should be imporved with collaboration with you to make this language is useful for future.

That's all from me, Thank you for review.