#include "math.h"
#include "stdio.h"
#include "xlut_exp.h"

int main() {
	float x = -10.0f;
	for(; x < 37.0f; x += 0.5f) {
		printf("%f = %f\n", xlut_exp(x), expf(x));
	}
}
