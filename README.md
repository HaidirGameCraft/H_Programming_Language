# H Programmer Language

Do you know when C file compile to binary it threw Assembly Language before compiling into binary?. So, this is my own CPU Emulator that inspired me from Intel x86 Architecture and Assembly Language.

H Programmer Language is the Emulator machine that running the H code to binary code.
This project has bug and the confusion of compiler and instruction program. *Dont Expect it Work Properly*. I will adjust it for next time.

# The Instruction set Format
the Instruction Set Format is the rule that make machine run functionlly

For this format:
- Including Offset:

|| Prefix | Ext Prefix (Optional) | Opcode | Register | Offset | Value |
| --- | :---: | :---: | :---: | :---: | :---: | :---: |
| bits | 8 | 8 | 8 | 8 | 16 | 32 |
| bytes | 1 | 1 | 1 | 1 | 2 | 4 |

- Not Include Offset:

|| Prefix | Ext Prefix (Optional) | Opcode | Register | Sym | Reserved | Value |
| --- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| bits | 8 | 8 | 8 | 8 | 4 | 12 | 32 |
| bytes | 1 | 1 | 1 | 1 | | 2 | 4 |



- Prefix are require for instruction set, containing flags
- Ext Prefix are option based on PREFIX_EXT_INC on Prefix
- Opcode are require for instruction set, containing operation code like MOV, PUSH, POP, ADD, SUB, MUL and so on.
- RegMem are optional because it containing Register format:
    - 4 (first) bits -> destination register
    - 4 (last) bits -> source register
- Offset are optional but when PREFIX.O == 1, it turn to required. basically, it use only 16 bits (2 bytes)
- Value are optional that containing value of data

# New Update
## Cross Platform
-   H programming Language are supported Windows and Linux
## Add
-   new Opcode PNT (reg/value/label) -> printing the text to terminal interface
-   new Opcode PCT (reg/value/label) -> printing the character (char) to terminal interface
-   Compiling Multiple Files
-   adding region TEXT, DATA, RAW
## Remake
-   Remaking all Instruction Set Format, Including:
    - new register will support only 32 bits and no longer using r8 and r16
    - new name register:
        |No (Hex) | Name Register | Bits | Description |
        | --- | :---:| :---: | :---: |
        | 0 | ra | 32 | Accumulator Register |
        | 1 | rb | 32 | Reserved Register |
        | 2 | rc | 32 | Counter Register |
        | 3 | rd | 32 | Reserved Register |
        | 4 | re | 32 | Reserved Register |
        | 5 | rf | 32 | Reserved Register |
        | 6 | rg | 32 | Reserved Register |
        | 7 | rh | 32 | Reserved Register |
        | 8 | ri | 32 | Remainer Register |
        | 9 | rj | 32 | Reserved Register |
        | A | rk | 32 | Reserved Register |
        | B | rl | 32 | Reserved Register |
        | C | rm | 32 | Reserved Register |
        | D | rn | 32 | Reserved Register |
        | E | rs | 32 | Stack Pointer Register |
        | F | rp | 32 | Base Pointer Register |
    - new instruction format. you can check on this section [INSTRUCTION FORMAT](#the-instruction-set-format)
    - repattern the instruction for compiler

# Example Code 
You can check the [(sample code)](/sample.hm)

# How to run the code
For the Compiler:
```sh
./hc -c <file.hm> -o <output_file.ho>
```

run the program:
```sh
./h <file.ho>
```

# Inspire
- This project inspire with:
    - Intel x86 Documentation: [(Link Documentation)](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

# Conclusion
The H Programmer Language is based on Assembly Language but different scheme to run the code. This project should be imporved with collaboration with you to make this language is useful for future.

That's all from me, Thank you for review.

## Disclaimer

The H Programming Language is provided **"as is"** for personal testing and learning only.

You may **download, build, and run** the project, but you may **not copy, reuse, or redistribute**
the source code without permission.

The author is **not responsible** for any damage or issues caused by using this software.
Use at your own risk.

