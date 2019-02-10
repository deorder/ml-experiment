#include<math.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>

#include "xcommon.h"
#include "ml_opencl.h"

XPUBLIC size_t ml_render_buffer_node(ml_token_node_t *node, size_t position, char *buffer) {
	switch(node->token_type) {
		case ML_TOKEN_TYPE_EXPRESSION: {
			size_t argument_index = 0;
			const char *format_ptr = node->expression_node.format;
			while(*format_ptr) {
				if(*format_ptr == '%') {
					format_ptr++;
					switch(*format_ptr) {
						case 's':
						position = ml_render_buffer_node(node->expression_node.nodes[argument_index], position, buffer) - 1;
						argument_index++;
						break;
						default:
						if(buffer) buffer[position] = *format_ptr;
						break;
					}
				} else {
					if(buffer) buffer[position] = *format_ptr;
				}
				format_ptr++;
				position++;
			}
		}
		break;

		case ML_TOKEN_TYPE_INPUT: {
			char input[1024]; size_t input_size;
			snprintf(input, sizeof(input), "%s[i]", node->input_node.name);
			input_size = strnlen(input, sizeof(input));
			if(buffer) memcpy((buffer + position), input, input_size);
			position += input_size;
		}
		break;

		default:
		break;
	}
	return position;
}

char* ml_render_buffer(ml_token_node_t node) {
	char *buffer; size_t buffer_size = 0;
	buffer_size = ml_render_buffer_node(&node, 0, NULL);
	buffer = malloc((buffer_size + 1) * sizeof(char));
	ml_render_buffer_node(&node, 0, buffer);
	buffer[buffer_size + 1] = '\0';
	return buffer;
}
