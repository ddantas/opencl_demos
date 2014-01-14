#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <opencv2/core/types_c.h>

void cl_error(int error);
void cl_ImInvert(IplImage* img);

void cl_ImThreshould(IplImage* img, char a){
	cl_uint num_opencl;
	cl_int err;
	size_t t_info;
	cl_char* info;
	
	/*PLATFORM STAGE*/
	cl_platform_id *plataforma;
	clGetPlatformIDs(3, NULL, &num_opencl);
	plataforma = (cl_platform_id*)malloc(sizeof(cl_platform_id)*
											num_opencl);										
	err = clGetPlatformIDs(num_opencl, plataforma, NULL);
	printf("PLATFORM STATUS: "); cl_error(err);
	if(err<0)
		return;
	for(int i=0; i< num_opencl; i++){
		clGetPlatformInfo(plataforma[i], CL_PLATFORM_NAME, 0,
								NULL, &t_info);
		info = (cl_char*)malloc(sizeof(cl_char)*t_info);
		clGetPlatformInfo(plataforma[i], CL_PLATFORM_NAME, t_info,
								info, NULL);
		printf("PLATFORM NAME %s\n", info);
	}
	//SE HOUVER UM NUMERO DE PLATAFORMAS MAIOR QUE UM O USUARIO DEVE
	//ESCOLHER QUAL QUER UTILIZAR, POR PADRÂO USAREMOS A 1º ENCONTRADA
	t_info=0;
	free(info);
	err=0;
	num_opencl=0;
	/*END OF PLATFORM STAGE*/
	
	/*DEVICE STAGE*/
	cl_device_id *device;
	clGetDeviceIDs(plataforma[0], CL_DEVICE_TYPE_GPU, 3, NULL, &num_opencl);
	if(num_opencl<=0){
		clGetDeviceIDs(plataforma[0], CL_DEVICE_TYPE_CPU, 3, NULL, &num_opencl);
		device = (cl_device_id*)malloc(sizeof(cl_device_id)*num_opencl);
		err = clGetDeviceIDs(plataforma[0], CL_DEVICE_TYPE_CPU, num_opencl, device, NULL);
		printf("DEVICE STATUS: "); cl_error(err);
		if(cl_error<=0)
			return;
	}else{
		clGetDeviceIDs(plataforma[0], CL_DEVICE_TYPE_GPU, 3, NULL, &num_opencl);
		device = (cl_device_id*)malloc(sizeof(cl_device_id)*num_opencl);
		err = clGetDeviceIDs(plataforma[0], CL_DEVICE_TYPE_GPU, num_opencl, device, NULL);
		printf("DEVICE STATUS: "); cl_error(err);
		if(cl_error<=0)
			return;
	}
	for(int i=0; i< num_opencl; i++){
		clGetDeviceInfo(device[i], CL_DEVICE_NAME, 0,
								NULL, &t_info);
		info = (cl_char*)malloc(sizeof(cl_char)*t_info);
		clGetDeviceInfo(device[i], CL_DEVICE_NAME, t_info,
								info, NULL);
		printf("DEVICE NAME %s\n", info);
	}
	/*END DEVICE STAGE*/
	/*CONTEXT STAGE*/
	cl_context context = clCreateContext(0, num_opencl, device, NULL,
						NULL, &err);
	printf("CONTEXT STATUS: "); cl_error(err);
	if(err<=0)
		return;
	/*END CONTEXT STAGE*/
	/*PROGRAMS STAGE*/
	FILE* program_handle = fopen("./CL/threshold.cl", "r");
	fseek(program_handle, 0, SEEK_END);
	size_t program_size = ftell(program_handle);
	rewind(program_handle);
	
	char* program_buffer = (char*)malloc(program_size+1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
	
	cl_program program = clCreateProgramWithSource(context, 1,
						 (const char**)program_buffer,
						 (const size_t*)program_size, &err);
	printf("PROGRAM STATUS "); cl_error(err);
	char result[1024];
	clBuildProgram(program, num_opencl, device, NULL, NULL, result);
	printf("RESULTS OF BUILDING:\n%s\n", result);
	/*END PROGRAM STAGE*/
	/*KERNEL STAGE*/
	const char kernel_name[] = "threshold";
	cl_kernel kernel = clCreateKernel(program[0], program_buffer, kernel_name, &err);
	printf("KERNEL STATUS: "); cl_error(err);
	/*END KERNEL STAGE*/
	/*COMMAND QUEUE STAGE*/
	cl_mem bufA;
	cl_mem bufB;
	
	cl_command_queue queue = clCreateCommandQueue(context, device[0], 0, &err);
	printf("QUEUE STATUS:   "); cl_error(err);	
	
	bufA = clCreateBuffer(context, CL_MEM_READ_WRITE,
			(sizeof(char)*(img->width*img->height)), NULL, &err);
	printf("BUF A:     "); cl_error(err);
	bufB = clCreateBuffer(context, CL_MEM_READ_ONLY,
			(sizeof(char)), NULL, &err);
	printf("BUF B:     "); cl_error(err);
	
	clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0,
	(sizeof(char)*(img->width*img->height)), img->imageData,
	0, NULL, NULL);
	
	clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0,
	sizeof(char), &a, 0, NULL, NULL);
	
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
	size_t sizeofprogram = {sizeof(char)*img->width*img->height};
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, 
						sizeofprogram, NULL, 0, NULL, NULL);
	
	clFinish(queue);
	
	clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0,
	(sizeof(char)*(img->width*img->height)),
	img->imageData, 0, NULL, NULL);
	printf("ENQUEUE BUFFER: ");
	cl_error(err);
	
	clReleaseMemObject(bufA);
	clReleaseMemObject(bufB);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
}
	

void cl_ImInvert(IplImage* img){
	cl_platform_id platformId;
	cl_device_id deviceId;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem bufB;
	cl_mem bufC;
	int tam = img->width*img->height;
	
	cl_int err, num;
	char* info;
	int* size = (int*) malloc(sizeof(int));
		
	char* hostC = (char*)malloc(tam*sizeof(char));
	
	size_t globalSize[1] = { tam };

	const char* fonte =
	"	__kernel void Invert( \
	__global const char* b, \
	__global char* c) \
	{ \
		int id = get_global_id(0); \
		c[id] = 255 - b[id]; \
	}";
	
	
	err = clGetPlatformIDs(5, NULL, &num);
	printf("TESTE DE PLATFORM... ");
	cl_error(err);
	if(err == 0){
		err = clGetPlatformIDs(num, &platformId, NULL);
		printf("Definindo platform... ");
		cl_error(err);
		printf("Exibir Informacao da Plataforma: \n");
		printf("**Nome da Plataforma: ");
		clGetPlatformInfo(platformId, CL_PLATFORM_NAME, 0, NULL, size);
		info = (char*) malloc(size[0]*sizeof(char));
		clGetPlatformInfo(platformId, CL_PLATFORM_NAME, size[0], info, NULL);
		printf("%s\n", info);
		free(info);
	}
	else{
		cl_error(err);
		//exit() aqui é para descobrir porque o exit() nao funciona
	}
		 
	
	printf("TESTE DE DISPOSITIVOS\n");
	err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 5, NULL, &num);
	if(err != 0){
		printf("Nenhuma GPU encontrada... \n");
		err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_CPU, 5, NULL, &num);
		if(err != 0)
			printf("Nenhuma CPU encontrada...\n");
		err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_CPU, num, &deviceId, NULL);
		printf("DEVICE: ");
		cl_error(err);
	}else{
		err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, num, &deviceId, NULL);
		printf("DEVICE: ");
		cl_error(err);
	}
	if(err == 0){
		printf("Informacoes do Dispositivo:\n");
		clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, size);
		info = (char*) malloc(size[0]*sizeof(char));
		clGetDeviceInfo(deviceId, CL_DEVICE_NAME, (sizeof(char)*size[0]), info, NULL);
		printf("**Nome do Dispositivo: %s\n", info);
		free(info);
	}
	
	context = clCreateContext(0, 1, &deviceId, NULL, NULL, &err);
	printf("CONTEXT: ");
	cl_error(err);

	queue = clCreateCommandQueue(context, deviceId, 0, &err);
	printf("QUEUE:   ");
	cl_error(err);	
	
	program = clCreateProgramWithSource(context, 1, &fonte, NULL, &err);
	printf("PROGRAM:  ");
	cl_error(err);
	
	clBuildProgram(program, 0, NULL, NULL, NULL, &err);
	printf("BUILD:    ");
	cl_error(err);
	
	kernel = clCreateKernel(program, "Invert", &err);
	printf("KERNEL    ");
	cl_error(err);
	
	
	bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, (tam*sizeof(char)), NULL, &err);
	printf("BUF B:     ");
	cl_error(err);
	bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, (tam*sizeof(char)), NULL, &err);
	printf("BUF C:     ");
	cl_error(err);
	clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, tam*sizeof(char), img->imageData, 0, NULL, NULL);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufB);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufC);
	
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
	
	clFinish(queue);
	
	clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, tam*sizeof(char), hostC, 0, NULL, NULL);
	printf("ENQUEUE BUFFER: ");
	cl_error(err);
	
	for(int i=0; i<tam; i++)
		img->imageData[i] = hostC[i];

	clReleaseMemObject(bufB);
	clReleaseMemObject(bufC);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
}

void cl_error(int error){
	switch(error){
		case 0:
			printf("CL_SUCCESS\n");
			break;
		case -1:
			printf("CL_DEVICE_NOT_FOUND\n");
			break;
		case -2:
			printf("CL_DEVICE_NOT_AVAILABLE\n");
			break;
		case -3:
			printf("CL_COMPILER_NOT_AVAILABLE\n");
			break;
		case -4:
			printf("CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
			break;
		case -5:
			printf("CL_OUT_OF_RESOURCES\n");
			break;
		case -6:
			printf("CL_OUT_OF_HOST_MEMORY\n");
			break;
		case -7:
			printf("CL_PROFILING_INFO_NOT_AVAILABLE\n");
			break;
		case -8:
			printf("CL_MEM_COPY_OVERLAP\n");
			break;
		case -9:
			printf("CL_IMAGE_FORMAT_MISMATCH\n");
			break;
		case -10:
			printf("CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
			break;
		case -11:
			printf("CL_BUILD_PROGRAM_FAILURE\n");
			break;
		case -12:
			printf("CL_MAP_FAILURE\n");
			break;
		case -13:
			printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
			break;
		case -14:
			printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
			break;
		case -15:
			printf("CL_COMPILE_PROGRAM_FAILURE\n");
			break;
		case -16:
			printf("CL_LINKER_NOT_AVAILABLE\n");
			break;
		case -17:
			printf("CL_LINK_PROGRAM_FAILURE\n");
			break;
		case -18:
			printf("CL_DEVICE_PARTITION_FAILED\n");
			break;
		case -19:
			printf("CL_KERNEL_ARG_INFO_NOT_AVAILABLE\n");
			break;
		case -30:
			printf("CL_INVALID_VALUE\n");
			break;
		case -31:
			printf("CL_INVALID_DEVICE_TYPE\n");
			break;
		case -32:
			printf("CL_INVALID_PLATFORM\n");
			break;
		case -33:
			printf("CL_INVALID_DEVICE\n");
			break;
		case -34:
			printf("CL_INVALID_CONTEXT\n");
			break;
		case -35:
			printf("CL_INVALID_QUEUE_PROPERTIES\n");
			break;
		case -36:
			printf("CL_INVALID_COMMAND_QUEUE\n");
			break;
		case -37:
			printf("CL_INVALID_HOST_PTR\n");
			break;
		case -38:
			printf("CL_INVALID_MEM_OBJECT\n");
			break;
		case -39:
			printf("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n");
			break;
		case -40:
			printf("CL_INVALID_IMAGE_SIZE\n");
			break;
		case -41:
			printf("CL_INVALID_SAMPLER\n");
			break;
		case -42:
			printf("CL_INVALID_BINARY\n");
			break;
		case -43:
			printf("CL_INVALID_BUILD_OPTIONS\n");
			break;
		case -44:
			printf("CL_INVALID_PROGRAM\n");
			break;
		case -45:
			printf("CL_INVALID_PROGRAM_EXECUTABLE\n");
			break;
		case -46:
			printf("CL_INVALID_KERNEL_NAME\n");
			break;
		case -47:
			printf("CL_INVALID_KERNEL_DEFINITION\n");
			break;
		case -48:
			printf("CL_INVALID_KERNEL\n");
			break;
		case -49:
			printf("CL_INVALID_ARG_INDEX\n");
			break;
		case -50:
			printf("CL_INVALID_ARG_VALUE\n");
			break;
		case -51:
			printf("CL_INVALID_ARG_SIZE\n");
			break;
		case -52:
			printf("CL_INVALID_KERNEL_ARGS\n");
			break;
		case -53:
			printf("CL_INVALID_WORK_DIMENSION\n");
			break;
		case -54:
			printf("CL_INVALID_WORK_GROUP_SIZE\n");
			break;
		case -55:
			printf("CL_INVALID_WORK_ITEM_SIZE\n");
			break;
		case -56:
			printf("CL_INVALID_GLOBAL_OFFSET\n");
			break;
		case -57:
			printf("CL_INVALID_EVENT_WAIT_LIST\n");
			break;
		case -58:
			printf("CL_INVALID_EVENT\n");
			break;
		case -59:
			printf("CL_INVALID_OPERATION\n");
			break;
		case -60:
			printf("CL_INVALID_GL_OBJECT\n");
			break;
		case -61:
			printf("CL_INVALID_BUFFER_SIZE\n");
			break;
		case -62:
			printf("CL_INVALID_MIP_LEVEL\n");
			break;
		case -63:
			printf("CL_INVALID_GLOBAL_WORK_SIZE\n");
			break;
		case -64:
			printf("CL_INVALID_PROPERTY\n");
			break;
		case -65:
			printf("CL_INVALID_IMAGE_DESCRIPTOR\n");
			break;
		case -66:
			printf("CL_INVALID_COMPILER_OPTIONS\n");
			break;
		case -67:
			printf("CL_INVALID_LINKER_OPTIONS\n");
			break;
		case -68:
			printf("CL_INVALID_DEVICE_PARTITION_COUNT\n");
			break;
		default:
			printf("NO VALID CL ERROR CODE\n");
	}
}
