# cerebrumcoitus

Note: This **will not work on Windows** due to the `pread()` function being exclusive to Unix-like systems.

Command line program that interprets and runs a file in the esoteric programming language [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck) (usually with file extension .bf).

Brainfuck was created by Urban Müller in 1993 and designed to be as minimalist and confusing as possible. It only has 8 valid symbols:
* `>` — increment the data pointer
* `<` — decrement the data pointer
* `+` — increment the value in the data pointer
* `-` — decrement the value in the data pointer
* `.` — write the value in the data pointer to standard output
* `,` — read one byte from standard input and store it in the data pointer
* `[` — jump ahead to the matching `]` if the value in the data pointer is 0
* `]` — jump behind to the matching `[` if the value in the data pointer is not 0
All other characters are ignored and used as comments.

Building requires a C compiler, preferably GCC, and Make. In the terminal, just type `make` to build, `./cerebrumsex <file>` to execute, and `make clean` to clean.

Linux executable size: 16.3 KiB
