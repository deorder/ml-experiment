#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdint.h>

#define LUT_TAG EXP
#define LUT_SIZE 256
#define LUT_NAME exp
#define LUT_TYPE float
#define LUT_FORMAT "%ff"
#define LUT_FUNC(x) expf(x)
#define LUT_TYPE_MIN FLT_MIN
#define LUT_TYPE_MAX FLT_MAX
#define LUT_RANGE_MIN FLT_MIN_10_EXP
#define LUT_RANGE_MAX FLT_MAX_10_EXP
#define LUT_INDEX_TYPE uint8_t
#define LUT_FILE "xlut_exp.h"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define CLAMP(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))

#define LUT_RANGE_SCALE ((LUT_TYPE)(LUT_RANGE_MAX - LUT_RANGE_MIN) / (LUT_TYPE)(LUT_SIZE))

int main() {
	size_t index = 0;

	FILE *file = fopen(LUT_FILE, "w");

	fprintf(file, "#include \"stdint.h\"\n\n");
	fprintf(file, "#ifndef _XLUT_" STR(LUT_TAG) "_H_\n");
	fprintf(file, "#define _XLUT_" STR(LUT_TAG) "_H_\n\n");

	fprintf(file, "#ifdef __cplusplus\n");
	fprintf(file, "extern \"C\" {\n");
	fprintf(file, "#endif\n");

	fprintf(file, "\n");

	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_SIZE %d\n", LUT_SIZE);
	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_RANGE_MIN " STR(LUT_RANGE_MIN) "\n");
	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_RANGE_MAX " STR(LUT_RANGE_MAX) "\n");
	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_RANGE_SCALE " STR(LUT_RANGE_SCALE) "\n");
	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_TO_INDEX(x) (" STR(LUT_INDEX_TYPE) ")((x - XLUT_" STR(LUT_TAG) "_RANGE_MIN) / XLUT_" STR(LUT_TAG) "_RANGE_SCALE)\n");
	fprintf(file, "#define XLUT_" STR(LUT_TAG) "_FROM_INDEX(i) (" STR(LUT_TYPE) ")(XLUT_" STR(LUT_TAG) "_RANGE_MIN + ((" STR(LUT_TYPE) ")(i) * XLUT_" STR(LUT_TAG) "_RANGE_SCALE))\n");

	fprintf(file, "\n");

	fprintf(file, "const " STR(LUT_TYPE) " xlut_" STR(LUT_NAME) "_table[%d] = {", LUT_SIZE);

	for(index = 0; index < LUT_SIZE; index++) {
		LUT_TYPE scalar = (LUT_TYPE)LUT_FUNC(LUT_RANGE_MIN + ((LUT_TYPE)(index) * LUT_RANGE_SCALE));
		
		if(index % 4 == 0) {
			fputs("\n\t", file);
		}

		fprintf(file, LUT_FORMAT ", ", scalar);
	}

	fputs("\n};\n", file);

	fprintf(file, "\n");
	fprintf(file, "static inline __inline__ __attribute__((always_inline)) " STR(LUT_TYPE) " xlut_" STR(LUT_NAME) "(const " STR(LUT_TYPE) " x) {\n");
	fprintf(file, "\t" STR(LUT_INDEX_TYPE) " i = XLUT_" STR(LUT_TAG) "_TO_INDEX(x);\n");
	fprintf(file, "\t" STR(LUT_TYPE) " p1 = xlut_" STR(LUT_NAME) "_table[i], p2 = xlut_" STR(LUT_NAME) "_table[i + 1];\n");
	fprintf(file, "\treturn p1 + ((p2 - p1) * (x - XLUT_" STR(LUT_TAG) "_FROM_INDEX(i)));\n");
	//fprintf(file, "\treturn p1;\n");
	fprintf(file, "}\n");
	fprintf(file, "\n");

	fprintf(file, "#ifdef __cplusplus\n");
	fprintf(file, "}\n");
	fprintf(file, "#endif\n\n");

	fprintf(file, "#endif // _XLUT_" STR(LUT_TAG) "_H_\n");

	fclose(file);

	return 0;
}
