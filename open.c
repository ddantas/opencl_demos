#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#define ARRAY_LENGTH 1000

int main(int argc, char** argv){
  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_context context;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;
  cl_mem bufA;
  cl_mem bufB;
  cl_mem bufC;

  int* hostA;
  int* hostB;
  int* hostC;
  size_t globalSize[1] = { ARRAY_LENGTH };
  int i;

  const char* source =
    "__kernel void ArrayDiff(\
__global const int* a, \
__global const int* b, \
__global int* c){\
int id = get_global_id(0);\
c[id]=a[id]-b[id];}";

  int* numPlatforms;
  //Obtenção de identificadores de platform e device
  clGetPlatformIDs(1, &platformId, &numPlatforms);
  printf("\nNum of platforms found %d", numPlatforms);
  //Argumentos da função clGetPlatformIDs
  //clGetPlatformIDs(<num de platform desejada>, <local onde os identificadores das platforms devem ser escritos>, <num de platform encontradas>);
  int* numDeviceCpu;
  clGetDeviceIDs(platformId, CL_DEVICE_TYPE_CPU, 1, &deviceId, &numDeviceCpu);
  printf("\nNum of Device of CPU type found %d", numDeviceCpu);
  //Arguments of clGetDeviceIDs function
  //clGetDeviceIDs(<identificador de platform>, <tipo do device>, <num de disp desejados>, <array onde serão armazenados\
  //os identificadores dos dispositivos encontrados>, <num de dispositivos encontrados>);
  
  //Criação do Context
  context = clCreateContext(0, 1, &deviceId, NULL, NULL, NULL);
  //Args of clCreateContext(<lista de propriedades para o contexto>, <num de devices para o contexto>, <lista dos identificadores dos devices>,
  //<ponteiro para uma função de notificação chamada se houver um erro no context>,
  //<ponteiro para dados arbitrários passados para a função de notificação>,
  //<local para armazenamentos do codigo de erro da chamada>);
  //retorna Context OpenCl.

  //Criação da Fila de Comandos ou Queue
  queue = clCreateCommandQueue(context, deviceId, 0, NULL);
  //Args clCreateCommandQueue(<context OpenCl>, <identificador device>, <propriedades da fila de commandS>, <local para armazenamento
  //do código de erro da chamada>);
  //retorna Fila de Commandos(CommandQueue)
  
  //Criação do objeto de programa(criar codigo-objeto do kernel)
  program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
  //Args clCreateProgramWithSource(<context>, <num de strings no array>, <array de strings do codigo-fonte a ser compilado>,
  //<array contendo os tamanhos das strings do array>, <local para armazenamento do codigo de erro>);
  //retorna Objeto do Programa
  
  //compilar o codigo-objeto do kernel para todos os devices do context
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  //Args clBuildProgram(<codigo-objeto>, <num de dispositivos para o qual o programa deve ser compilado (0 compila para todos presentes no context)>,
  //<lista de devices para compilar (NULL indica todos)>, <string de opções para o compilador>, <ponteiro para função de notificação de chamada após
  //compilação>, <ponteiro para dados arbitrários passados para função de notificação>);
  //retorna codigo de erro

  //Obtenção de um kernel a partir do programa compilado
  kernel = clCreateKernel(program, "ArrayDiff", NULL);
  //Args clCreatKernel(<codigo-objeto>, <nome da função __kernel>, <local para armazenamento de codigo de erro>);
  //retorna Kernel

  //Alocação e inicialização dos arrays nos hosts comum em c
  hostA = (int*) malloc(ARRAY_LENGTH * sizeof(int));
  hostB = (int*) malloc(ARRAY_LENGTH * sizeof(int));
  hostC = (int*) malloc(ARRAY_LENGTH * sizeof(int));

  for(i=0; i<ARRAY_LENGTH;++i){
    hostA[i]=rand() % 1001 - 500;
    hostB[i]=rand() % 1001 - 500;
  }

  //Criação dos objetos de memória para comunicação com a memória global do device encontrado
  bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_LENGTH * sizeof(int), NULL, NULL);
  //Args clCreateBuffer(<context>, <flags (CL_MEM_READ_ONLY ou CL_MEM_READ_WRITE)>, <tamanho em bytes do buffer a ser alocado>, 
  //<local para armazenamento de codigo de erro>);
  //retorna Objeto de Memória
   bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_LENGTH * sizeof(int), NULL, NULL);
   bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_LENGTH * sizeof(int), NULL, NULL);
   
   //Transferencia dos arrays de entrada para mem do device
   clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, ARRAY_LENGTH * sizeof(int), hostA, 0, NULL, NULL);
   //Args clEnqueueWriteBuffer(<fila de commandos(queue)>, <objeto de memoria tipo buffer>, <bloquear ou não a escrita(CL_TRUE=bloqueante,
   // CL_FALSE=não-bloqueante)>, <offset a partir do qual os dados devem ser transferidos>, <tamanho em bytes dos dados a serem transferidos>,
   //<ponteiro para host>, <num de eventos a serem guardados antes do inicio da transferencia de dados>, <lista de eventos do argumento anterior>,
   //<local para retorno do objeto de envento para o comando>);
   //retorna codigo de erro

   //Configurar argumentos do kernel
   clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
   //Args clSetKernelArg(<kernel cujo argumento deve ser configurado>, <posição do argumento iniciado em 0>, <tamanho dos dados do argumento>,
   //<ponteiro para dados dos argumentos>);
   clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
   clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
   
   //Enviando o kernel para execução no device
   clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, NULL, 0, NULL, NULL);
   //Args cl...Kernel(<queue>, <kernel a ser executado>, <num de dimensoes do espaço de indices>, <array de deslocamentos para valores dos indices
   //em cada dimensão>, <array contendo os tamanhos para cada dimensão do espaço de indices>, <Array de tamanhos dos workgrups em cada dimensão(
   //NULL deixa automatico)>, <num de eventos da lista que devem ser aguardados antes do início da execução do kernel>, <lista de eventos do arg
   //anterior>, <local para retorno do objeto de evento para o comando>);
   //retorna codigo de erro

   //Sincronização (bloqueia o host até termino da execução do kernel
   clFinish(queue);
   //Args clFinish(<fila de comandos(queue)>);
   
   //Transferência dos resultados para mem do host
   clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, ARRAY_LENGTH * sizeof(int), hostC, 0, NULL, NULL);
   //ARgs cl...Buffer(<fila de comando>, <objeto de mem do tipo buffer>, ..., <offset a partir do qual os dados devem ser transferidos>
   //<tamanho em bytes dos dados a serem transferidos>, <ponteiro para host a ser escrito>, <num de eventos a ser aguardados>, <lista de eventos
   //a serem aguardados>, <local para retorno do objeto de evento>);
   //retorna codigo de erro

   //Imprimir resultados comum c
   printf("\n\nIMPRESSÃO DE RESULTADOS\n");
   for(i=0;i<ARRAY_LENGTH;++i)
     printf("%d - %d = %d", hostA[i], hostB[i], hostC[i]);

   //Liberação de recursos
   clReleaseMemObject(bufA);
   clReleaseMemObject(bufB);
   clReleaseMemObject(bufC);
   clReleaseKernel(kernel);
   clReleaseProgram(program);
   clReleaseCommandQueue(queue);
   clReleaseContext(context);

   //liberação de memoria comum c
   free(hostA);
   free(hostB);
   free(hostC);

   return 0;
}
