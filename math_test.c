#include<math.h>
#include<stdio.h>
#include<stdint.h>
#include<xmmintrin.h>

#include "xcommon.h"
//#include "xlut_exp.h"

#define SCALAR_TYPE_FLOAT

typedef float xfloat_t;

#define XFLOAT_ONE 1.0f
#define XFLOAT_ZERO 0.0f
#define XFLOAT_HALF 0.5f

#define xfloat_log(x) logf(x)
#define xfloat_exp(x) expf(x)
//#define xfloat_exp(x) xlut_exp(x)

#define PUBLIC
#define PRIVATE static
#define INLINE __inline__ __attribute__((always_inline)) 
#define RESTRICT __restrict__
#define ALIGN(x) __attribute__((aligned(x)))
#define VECTOR(x) __attribute__((vector_size(x)))

#ifdef SCALAR_TYPE_VECTOR4
typedef __m128 xvector4_t;
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
typedef float xvector4_t VECTOR(16);
typedef ALIGN(16) struct xmatrix4_s {
	xvector4_t row[4];
} xmatrix4_t;
#endif

#ifdef SCALAR_TYPE_FLOAT
typedef ALIGN(16) struct xvector4_s {
	xfloat_t col[4];
} xvector4_t;

typedef ALIGN(16) struct xmatrix4_s {
	xvector4_t row[4];
} xmatrix4_t;
#endif

#ifdef SCALAR_TYPE_FLOAT
XINLINE xvector4_t xvector4_set(const xfloat_t x, const xfloat_t y, const xfloat_t z, const xfloat_t w) {
	xvector4_t r = {{x, y, z, w}};
	return r;
}
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
XINLINE xvector4_t xvector4_set(const xfloat_t x, const xfloat_t y, const xfloat_t z, const xfloat_t w) {
	xvector4_t r = {x, y, z, w};
	return r;
}
#endif

#ifdef SCALAR_TYPE_FLOAT
XINLINE xmatrix4_t xmatrix4_set(
		const xfloat_t s00, const xfloat_t s01, const xfloat_t s02, const xfloat_t s03,
		const xfloat_t s10, const xfloat_t s11, const xfloat_t s12, const xfloat_t s13,
		const xfloat_t s20, const xfloat_t s21, const xfloat_t s22, const xfloat_t s23,
		const xfloat_t s30, const xfloat_t s31, const xfloat_t s32, const xfloat_t s33
) {
	xmatrix4_t r =  {{
										{{s00, s01, s02, s03}},
										{{s10, s11, s12, s13}},
										{{s20, s21, s22, s23}},
										{{s30, s31, s32, s33}}
                  }};
	return r;
}
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
XINLINE xmatrix4_t xmatrix4_set(
		const xfloat_t s00, const xfloat_t s01, const xfloat_t s02, const xfloat_t s03,
		const xfloat_t s10, const xfloat_t s11, const xfloat_t s12, const xfloat_t s13,
		const xfloat_t s20, const xfloat_t s21, const xfloat_t s22, const xfloat_t s23,
		const xfloat_t s30, const xfloat_t s31, const xfloat_t s32, const xfloat_t s33
) {
	xmatrix4_t r =  {{
										{s00, s01, s02, s03},
										{s10, s11, s12, s13},
										{s20, s21, s22, s23},
										{s30, s31, s32, s33}
                  }};
	return r;
}
#endif

#ifdef SCALAR_TYPE_FLOAT
XINLINE xfloat_t xvector4_dot(const xvector4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xfloat_t r = {XFLOAT_ZERO};
	for(index = 0; index < 4; index++) {
		r += xs.col[index] * ys.col[index];
	}
	return r;
}
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
XINLINE xfloat_t xvector4_dot(const xvector4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xfloat_t r = {XFLOAT_ZERO};
	for(index = 0; index < 4; index++) {
		r += xs[index] * ys[index];
	}
	return r;
}
#endif

#ifdef SCALAR_TYPE_FLOAT
XINLINE xvector4_t xmatrix4_vector4_mul(const xmatrix4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xvector4_t r = {{XFLOAT_ZERO}};
	for(index = 0; index < 4; index++) {
		r.col[index] = xvector4_dot(xs.row[index], ys);
	}
	return r;
}
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
XINLINE xvector4_t xmatrix4_vector4_mul(const xmatrix4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xvector4_t r = {XFLOAT_ZERO};
	for(index = 0; index < 4; index++) {
		r[index] = xvector4_dot(xs.row[index], ys);
	}
	return r;
}
#endif

#ifdef SCALAR_TYPE_FLOAT
XINLINE xvector4_t xvector4_element_mul(const xvector4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xvector4_t r = {{XFLOAT_ZERO}};
	for(index = 0; index < 4; index++) {
		r.col[index] = xs.col[index] * ys.col[index];
	}
	return r;
}
#endif

#ifdef SCALAR_TYPE_GCC_VECTOR4
XINLINE xvector4_t xvector4_element_mul(const xvector4_t xs, const xvector4_t ys) {
	size_t index = 0;
	xvector4_t r = {XFLOAT_ZERO};
	for(index = 0; index < 4; index++) {
		r[index] = xs[index] * ys[index];
	}
	return r;
}
#endif

xvector4_t math_test(const xmatrix4_t m, const xvector4_t v) {
	//return xmatrix4_vector4_mul(m, v);
	return xvector4_element_mul(v, v);
}

int main()
{
	#ifndef ASMTEST
	size_t index __attribute__((unused)) = 0;

	xvector4_t v = xvector4_set(1.0f, 2.0f, 3.0f, 4.0f);

	xmatrix4_t m = xmatrix4_set(
                  1.0f, 2.0f, 3.0f, 4.0f,
									5.0f, 6.0f, 7.0f, 8.0f,
									9.0f, 10.0f, 11.0f, 12.0f,
                  13.0f, 14.0f, 15.0f, 16.0f
                 );

	xvector4_t result = math_test(m, v);

	#ifdef SCALAR_TYPE_GCC_VECTOR4
	for(index = 0; index < 4; index++) {
		printf("%f\n", result[index]);
	}
	#endif

	#ifdef SCALAR_TYPE_FLOAT
	for(index = 0; index < 4; index++) {
		printf("%f\n", result.col[index]);
	}
	#endif

	#endif

	exit(0);
}
