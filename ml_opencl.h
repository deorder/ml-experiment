#pragma once

#ifndef _ML_OPENCL_H_
#define _ML_OPENCL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include<math.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include "xcommon.h"

typedef enum {
	ML_TOKEN_TYPE_NONE = 0,
	ML_TOKEN_TYPE_EXPRESSION,
	ML_TOKEN_TYPE_INPUT,
	ML_TOKEN_TYPE_COUNT
} ml_token_type_t;

typedef union ml_token_node_s ml_token_node_t;

typedef struct {
	ml_token_type_t token_type;
	char *name; void *data;
} ml_input_node_t;

typedef struct {
	ml_token_type_t token_type;
	char *format;
	ml_token_node_t *nodes[16];
} ml_expression_node_t;

typedef union ml_token_node_s {
	ml_token_type_t token_type;
	ml_input_node_t input_node;
	ml_expression_node_t expression_node;
} ml_token_node_t;

typedef ml_token_node_t ml_scalar_t;
typedef ml_token_node_t ml_vector_t;
typedef ml_token_node_t ml_matrix_t;

#define ML_INPUT(name, data) ((ml_token_node_t){.input_node = {ML_TOKEN_TYPE_INPUT, name, data}})
#define ML_EXPRESSION(format, ...) ((ml_token_node_t){.expression_node = {ML_TOKEN_TYPE_EXPRESSION, format, {__VA_ARGS__}}})

XPUBLIC size_t ml_render_buffer_node(ml_token_node_t *node, size_t position, char *buffer);
XPUBLIC char* ml_render_buffer(ml_token_node_t node);

XINLINE ml_scalar_t vector_dot(ml_vector_t xs, ml_vector_t ys) {
	return (ml_vector_t)ML_EXPRESSION("dot(%s, %s)", &xs, &ys);
}

XINLINE ml_vector_t matrix_vector_mul(ml_matrix_t xs, ml_vector_t ys) {
	return (ml_vector_t)ML_EXPRESSION("mul(%s, %s)", &xs, &ys);
}

XINLINE ml_vector_t vector_element_mul(ml_vector_t xs, ml_vector_t ys) {
	return (ml_vector_t)ML_EXPRESSION("(%s * %s)", &xs, &ys);
}

XINLINE ml_vector_t sigmoid(ml_vector_t xs) {
	return (ml_vector_t)ML_EXPRESSION("(1.0 / (1.0 + log(%s)))", &xs);
}

XINLINE ml_vector_t ml_vector(char *name, void *data) {
	return (ml_vector_t)ML_INPUT(name, data);
}

XINLINE ml_matrix_t ml_matrix(char *name, void *data) {
	return (ml_matrix_t)ML_INPUT(name, data);
}

#ifdef __cplusplus
}
#endif

#endif // _ML_OPENCL_H_
