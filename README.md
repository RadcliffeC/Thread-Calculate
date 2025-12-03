# Thread Calculate

A multithreading application to calculate total from integers in a binary file

## Use

To use, clone the repository and **cd** into that directory. 
once there, run:

```bash
gcc main.c -lpthread -pedantic -std=gnu89 -o threadCalc
```

This will compile the program. To run it, run the following command in the same directory:

```bash
./threadCalc <filename> <number>
```

**filename**: name of binary file to calculate
**number**: number of threads to read with

## Intention

This application is intended as a learning tool for C and multithreading. It is compilied under the GNU89 standard with the GCC compilier. A makefile is also provided which compiles under the C90 standard.