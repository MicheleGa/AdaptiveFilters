#include "pmsis.h"
#include "math.h"
#include "data.h"

// common functions
int check(float * result, float * ground_truth, int n);
void print_array(float * arr, int n);
void print_array_2d(float * arr, int n, int m);
float norm_L2(float * vect, int n);
void zeros(float * arr, int n);
void eye(float element, float * matrix, int n);
void gemv(int size_N, int size_M, float* mat_i, float* vec_1, float* vec_o);
void outer(int size_N, int size_M, float* vec_1, float* vec_2, float* matrix_o);
void mat_transpose(float * mat_in, float * mat_out, int sizeM, int sizeN);
void matMul(float * pSrcA, float  * pSrcB, float * pDstC, int m, int n, int o);
void matAdd(float * pSrcA, float  * pSrcB, float * pDstC, int m, int n, int o);