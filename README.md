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

# Documentation
This is documentation for this programming language [Documentation](DOCUMENTATION.md)

# ChangesLog
you can click this link to look the changes [ChangesLog](ChangesLogs/CHANGESLOG-01-17-26.md)

# Example Code 
You can check the [(sample code)](/sample.hm)

# How to run the code
For the Compiler:
```sh
# H High Level Language Compiler
./bin/hc --help

# to Compile
./bin/hc -o <output_file> -c <hi_files...> -cl <hm_files...>
```

run the program:
```sh
./bin/h --help

# Example
./bin/h ./sample.ho -sr
```

# Installation
Download from github:
```sh
git clone https://github.com/HaidirGameCraft/H_Programming_Langauge
cd H_Programming_Langauge
```
For Linux:
```sh
sh ./build-linux.sh
```

For (WSL) on Windows:
```sh
sh ./build-linux.sh
sh ./build-windows.sh
```

For Build All on (WSL) in Windows or Linux:
```sh
sh ./build_all.sh
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

