# Machine learning experiment using C99

## Description

This is just a collection of some machine learning experiments I did using C99.
At the moment there is nothing that does anything with machine learning, but this is just a start.

The files:
 - **ml_opencl.c/h:** Implementation of a DSL that can be used to generate OpenCL shader program code
 - **learn_opencl_test.c:** Test to generate OpenCL shader program code using the DSL implemented in `ml_opencl.c/h`
 - **learn_cpu_test.c:** Test to perform calculations on the CPU using a DSL (implemented in the same file for now)
 - **learn_opencl.c:** To compile and run shader programs generated with `learn_opencl_test` using OpenCL
 - **math_test.c:** Using this to test some of my math functions and generated ASM
 - **matvec.cl:** OpenCL shader program generated using `learn_opencl_test`
 - **xlut_exp.h:** Header file with the exp LUT that is generated during the build by `xlut_exp_gen`
 - **xlut_exp_gen.c:** Code to generate exponential lookup tables in `xlut_exp.h`
 - **xlut_exp_test.c:** For testing `xlut_exp_gen.c`

 
## Build

Run `make`

## Requirements
 - A compiler that supports C99
 - A OpenCL SDK
