#include<math.h>
#include<stdio.h>
#include<stdint.h>
#include<xmmintrin.h>

#include "xcommon.h"
#include "xlut_exp.h"
#include "ml_cpu.h"

#define SCALAR_TYPE_VECTOR4

#define ML_LOG(x) logf(x)
#define ML_EXP(x) expf(x)
//#define ML_EXP(x) xlut_exp(x)

#define ML_TYPE_ONE 1.0f
#define ML_TYPE_ZERO 0.0f
#define ML_TYPE_HALF 0.5f

#define ML_MAX_VECTOR_SIZE 128
#define ML_MAX_MATRIX_SIZE 128

#define PUBLIC
#define PRIVATE static
#define INLINE __inline__ __attribute__((always_inline)) 
#define RESTRICT __restrict__
#define ALIGN(x) __attribute__((aligned(x)))
#define VECTOR(x) __attribute__((vector_size(x)))

#ifdef SCALAR_TYPE_FLOAT
typedef struct {
	float x, y, z, w;
} ALIGN(16) vector4_t;
#endif

#ifdef SCALAR_TYPE_VECTOR4
typedef __m128 vector4_t;
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
typedef float vector4_t VECTOR(16);
#endif

typedef float ml_type_t;

typedef struct {
	size_t columns;
	ml_type_t data[ML_MAX_VECTOR_SIZE];
} ml_vector_t;

typedef struct {
	size_t rows, columns;
	ml_vector_t data[ML_MAX_MATRIX_SIZE];
} ml_matrix_t;

XINLINE ml_type_t vector_dot(ml_vector_t xs, ml_vector_t ys) {
	size_t index = 0;
	ml_type_t result = ML_TYPE_ZERO;
	for(index = 0; index < xs.columns; index++) {
		result += xs.data[index] * ys.data[index];
	}
	return result;
}

XINLINE ml_vector_t matrix_vector_mul(ml_matrix_t xs, ml_vector_t ys) {
	size_t index = 0;
	ml_vector_t result = {xs.rows, {0}};
	for(index = 0; index < xs.rows; index++) {
		result.data[index] = vector_dot(xs.data[index], ys);
	}
	return result;
}

XINLINE ml_vector_t vector_element_mul(ml_vector_t xs, ml_vector_t ys) {
	size_t index = 0;
	ml_vector_t result = {xs.columns, {0}};
	for(index = 0; index < xs.columns; index++) {
		result.data[index] = xs.data[index] * ys.data[index];
	}
	return result;
}

XINLINE ml_vector_t sigmoid(const ml_vector_t xs) {
	size_t index = 0;
	ml_vector_t result = {xs.columns, {0}};
	for(index = 0; index < xs.columns; index++) {
		result.data[index] = ML_TYPE_ONE / (ML_TYPE_ONE + ML_EXP(-xs.data[index]));
	}
	return result;
}

int main() {
	size_t index = 0;

	ml_vector_t v = {8, {-1, -0.1, -3, 4, 5, 6, -7, 8}}; // ys[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	ml_matrix_t m = {2, 8, {{8, {1, 1, 1, 1, 1, 1, 1, 1}}, {8, {0.5, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8}}}};
	
	ml_vector_t result = sigmoid(matrix_vector_mul(m, v));

	for(index = 0; index < result.columns; index++) {
		printf("%f\n", result.data[index]);
	}

	exit(0);
}
