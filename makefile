all: xlut_exp_test math_test math_test.s learn_opencl learn_cpu_test learn_opencl_test

INCLUDE_DIRS=common

.PHONY: clean

xlut_exp_gen: xlut_exp_gen.c
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -Wall -W xlut_exp_gen.c -lm -o xlut_exp_gen

xlut_exp.h: xlut_exp_gen
	./xlut_exp_gen

xlut_exp_test: xlut_exp_test.c xlut_exp.h
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -Wall -W xlut_exp_test.c -lm -o xlut_exp_test

math_test math_test.s: math_test.c xlut_exp.h
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -ffast-math -ftree-vectorize -ftree-slp-vectorize -ftree-vectorizer-verbose=2 -Wall math_test.c -lm -o math_test
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -ffast-math -DASMTEST -ftree-vectorize -ftree-slp-vectorize -ftree-vectorizer-verbose=2 -Wall -S math_test.c -lm -o math_test.s

learn_cpu_test: learn_cpu_test.c xlut_exp.h
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -Wall -W ml_cpu.c learn_cpu_test.c -lm -o learn_cpu_test

learn_opencl: learn_opencl.c
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -Wall -W learn_opencl.c -lm -lOpenCL -o learn_opencl

learn_opencl_test: learn_opencl_test.c
	gcc -O3 -I${INCLUDE_DIRS} -msse2 -Wall -W ml_opencl.c learn_opencl_test.c -lm -lOpenCL -o learn_opencl_test

clean:
	rm -f learn_opencl math_test math_test.s learn_cpu_test learn_opencl_test xlut_exp_gen xlut_exp.h xlut_exp_test
