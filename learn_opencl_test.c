#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ml_common.h"
#include "ml_opencl.h"

int main() {
	char *buffer;
	float xs[8] = {-1, -0.1, -3, 4, 5, 6, -7, 8}; // ys[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	float ts[8 * 2] = {1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

	buffer = ml_render_buffer(sigmoid(matrix_vector_mul(ml_vector("xs", xs), ml_matrix("ts", ts))));
	printf("output: \"%s\"\n", buffer);
	free(buffer);

	exit(0);
}
