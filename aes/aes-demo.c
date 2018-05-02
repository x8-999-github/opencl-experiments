#include "timing.h"

#include "cl-helper.h"

#include <string.h>


int main(int argc, char **argv)
{
  const cl_long n = 16;


  cl_context ctx;
  cl_command_queue queue;

  const char *cl_platform_name = getenv("OPENCL_PLATFORM");
  const char *cl_device_name = getenv("OPENCL_DEVICE");

  if (cl_platform_name == NULL){
      cl_platform_name = CHOOSE_INTERACTIVELY;
  }

  if (cl_device_name == NULL){
      cl_device_name = CHOOSE_INTERACTIVELY;
  }
  create_context_on(cl_platform_name, cl_device_name, 0, &ctx, &queue, 0);

  //print_device_info_from_queue(queue);

  // --------------------------------------------------------------------------
  // load kernels 
  // --------------------------------------------------------------------------
  char *knl_text = read_file("aes.cl");
  cl_kernel knl = kernel_from_string(ctx, knl_text, "kernel_aes128", NULL);
  free(knl_text);


  //void kernel_aes128(__global uchar* state, __constant const uchar * key, const uint mode)

  // --------------------------------------------------------------------------
  // allocate and initialize CPU memory
  // --------------------------------------------------------------------------
  unsigned char * state = (unsigned char *) malloc(16);
  if (!state) { perror ("alloc state");}
  unsigned char * key = (unsigned char *) malloc(16);
  if (!key) { perror ("alloc key");}

  //initialize data
  memcpy(state,"Two One Nine Two",16);
  memcpy(key,"Thats my Kung Fu",16);
  


  // --------------------------------------------------------------------------
  // allocate device memory
  // --------------------------------------------------------------------------
  cl_int status;
  cl_mem state_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE, 
      16, 0, &status);
  CHECK_CL_ERROR(status, "clCreateBuffer");

  cl_mem key_buf = clCreateBuffer(ctx, CL_MEM_READ_WRITE,
      16, 0, &status);
  CHECK_CL_ERROR(status, "clCreateBuffer");

  // --------------------------------------------------------------------------
  // transfer to device
  // --------------------------------------------------------------------------
  CALL_CL_GUARDED(clEnqueueWriteBuffer, (
        queue, state_buf, /*blocking*/ CL_TRUE, /*offset*/ 0,
        16, state,
        0, NULL, NULL));

  CALL_CL_GUARDED(clEnqueueWriteBuffer, (
        queue, key_buf, /*blocking*/ CL_TRUE, /*offset*/ 0,
        16, key,
        0, NULL, NULL));
  uint32_t aes_mode =0;
  uint32_t nr_blocks =1;
  // --------------------------------------------------------------------------
  // run code on device
  // --------------------------------------------------------------------------

  CALL_CL_GUARDED(clFinish, (queue));

  timestamp_type time1, time2;
  get_timestamp(&time1);

  SET_4_KERNEL_ARGS(knl, state_buf, nr_blocks, key_buf, aes_mode);
  //TODO increase the amount of kernels running etc
  size_t ldim[] = { 1 };
  size_t gdim[] = { 1 };
  CALL_CL_GUARDED(clEnqueueNDRangeKernel,
      (queue, knl,
        /*dimensions*/ 1, NULL, gdim, ldim,
        0, NULL, NULL));

  CALL_CL_GUARDED(clFinish, (queue));

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  printf("%f s\n", elapsed);
  printf("%f GB/s\n",
      3*n*sizeof(float)/1e9/elapsed);

  // --------------------------------------------------------------------------
  // transfer back & check
  // --------------------------------------------------------------------------
  CALL_CL_GUARDED(clEnqueueReadBuffer, (
        queue, state_buf, /*blocking*/ CL_TRUE, /*offset*/ 0,
        16, state,
        0, NULL, NULL));

   printf("State:");
   for (int i =0 ; i < 16 ; i++){
   	printf("%02X", state[i]);
   }
   printf("\n");

  // --------------------------------------------------------------------------
  // clean up
  // --------------------------------------------------------------------------
  CALL_CL_GUARDED(clReleaseMemObject, (state_buf));
  CALL_CL_GUARDED(clReleaseMemObject, (key_buf));
  CALL_CL_GUARDED(clReleaseKernel, (knl));
  CALL_CL_GUARDED(clReleaseCommandQueue, (queue));
  CALL_CL_GUARDED(clReleaseContext, (ctx));

  return 0;
}
