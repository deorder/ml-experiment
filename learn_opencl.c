#include <stdio.h>		
#include <stdlib.h>		
#ifdef __APPLE__		
#	include <OpenCL/opencl.h>		
#else		
#	include <CL/cl.h>		
#endif		

#define MAX_SOURCE_SIZE (1024 * 4)

int main() {

	cl_int errcode;

	size_t program_size;
	FILE *program_handle;
	char *program_buffer;

	cl_uint num_devices;
	cl_uint num_platforms;
	cl_uint num_entries = 1;

	char device_name[1024];
	char platform_name[1024];

	size_t work_units_per_kernel = 4;
	float mat[16], vec[4], result[4];
	float correct[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	cl_mem mat_buffer, vec_buffer, result_buffer;

	/*
	cl_event event;
	*/
	cl_kernel kernel = NULL;
	cl_program program = NULL;
	cl_context context = NULL;
	cl_device_id device_id = NULL;
	cl_platform_id platform_id = NULL;
	cl_command_queue command_queue = NULL;
	cl_context_properties *context_properties = NULL;

	// CL_DEVICE_TYPE_CPU, CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_ACCELERATOR, CL_DEVICE_TYPE_DEFAULT, CL_DEVICE_TYPE_ALL
	cl_device_type device_type = CL_DEVICE_TYPE_DEFAULT;

	size_t i = 0;
	for(i = 0; i < 16; i++) {
		mat[i] = i * 2.0f;
	}
	for(i = 0; i < 4; i++) {
		vec[i] = i * 3.0f;
		correct[0] += mat[i] * vec[i];
		correct[1] += mat[i + 4] * vec[i];
		correct[2] += mat[i + 8] * vec[i];
		correct[3] += mat[i + 12] * vec[i];
	}

	clGetPlatformIDs(num_entries, &platform_id, &num_platforms);

	errcode = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
	if(errcode == CL_SUCCESS) {
		printf("CL_PLATFORM_NAME = %s\n", platform_name);
	} else {
		printf("error: clGetPlatformInfo\n"); exit(EXIT_FAILURE);
	}

	clGetDeviceIDs(platform_id, device_type, num_entries, &device_id, &num_devices);

	errcode = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
	if(errcode == CL_SUCCESS) {
		printf("CL_DEVICE_NAME = %s\n", device_name);
	} else {
		printf("error: clGetDeviceInfo\n"); exit(EXIT_FAILURE);
	}

	context = clCreateContext(context_properties, num_devices, &device_id, NULL, NULL, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateContext\n"); exit(EXIT_FAILURE);
	}

	/*
	errcode = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_size), &local_size, NULL);
	if(errcode == CL_SUCCESS) {
		printf("CL_DEVICE_LOCAL_MEM_SIZE = %d\n", (int)local_size);
	} else {
		printf("error: clGetDeviceInfo\n"); exit(EXIT_FAILURE);
	}
	*/

	program_handle = fopen("matvec.cl", "r");
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle); 
	rewind(program_handle);
	/*
	program_buffer = (char*)malloc(MAX_SOURCE_SIZE + 1);
	program_size = fread(program_buffer, 1, MAX_SOURCE_SIZE, program_handle);
	*/
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	if(!(fread(program_buffer, sizeof(char), program_size, program_handle)) > 0) {
		printf("error: fread\n"); exit(EXIT_FAILURE);
	}
	fclose(program_handle);

	program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, (const size_t*)&program_size, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateProgramWithSource\n"); exit(EXIT_FAILURE);
	}

	free(program_buffer);

	errcode = clBuildProgram(program, num_devices, &device_id, "", NULL, NULL);
	if(errcode != CL_SUCCESS) {
		char *log; size_t log_size;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		log = malloc(log_size);
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("error: clBuildProgram:\n%s\n", log);
		free(log);
		exit(EXIT_FAILURE);
	}

	kernel = clCreateKernel(program, "matvec_mult", &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateKernel\n"); exit(EXIT_FAILURE);
	}

	command_queue = clCreateCommandQueue(context, device_id, 0, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateCommandQueue\n"); exit(EXIT_FAILURE);
	}


	mat_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 16, mat, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateBuffer mat\n"); exit(EXIT_FAILURE);
	}

  vec_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * 4, vec, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateBuffer vec\n"); exit(EXIT_FAILURE);
	}

  result_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * 4, NULL, &errcode);
	if(errcode != CL_SUCCESS) {
		printf("error: clCreateBuffer result %d\n", errcode); exit(EXIT_FAILURE);
	}

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &mat_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &vec_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &result_buffer);

	errcode = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &work_units_per_kernel, NULL, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) {
		printf("error: clEnqueueNDRangeKernel\n"); exit(EXIT_FAILURE);
	}

	errcode = clEnqueueReadBuffer(command_queue, result_buffer, CL_TRUE, 0, sizeof(float) * 4, result, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) {
		printf("error: clEnqueueReadBuffer\n"); exit(EXIT_FAILURE);
	}

	if((result[0] == correct[0]) && (result[1] == correct[1]) && (result[2] == correct[2]) && (result[3] == correct[3])) {
		printf("SUCCESS\n");	
	} else {
		printf("FAILED\n");
	}

	/*
	errcode = clEnqueueTask(command_queue, kernel, 0, NULL, &event);
	if(errcode == CL_OUT_OF_RESOURCES) {
		printf("error: clEnqueTask: Out of Resources\n"); exit(EXIT_FAILURE);
	}

	errcode = clWaitForEvents(1, &event);
	if(errcode != CL_SUCCESS) {
		printf("error: clWaitForEvents\n"); exit(EXIT_FAILURE);
	}
	*/

	clReleaseMemObject(result_buffer);
	clReleaseMemObject(vec_buffer);
	clReleaseMemObject(mat_buffer);

	clReleaseCommandQueue(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseContext(context);

	exit(EXIT_SUCCESS);

}
