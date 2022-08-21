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
void gemv(float* mat_i, float* vec_1, int size_N, int size_M, float* vec_o);
void outer(float* vec_1, float* vec_2, int size_N, int size_M, float* matrix_o);
void mat_transpose(float * mat_in, int sizeN, int sizeM, float * mat_out);
void mat_mul_f32(float * pSrcA, float  * pSrcB, int m, int n, int o, float * pDstC);
void mat_add_f32(float * pSrcA, float * pSrcB, uint32_t M, uint32_t N, float * pDst);
int mat_inv_f32(float * pSrc, float * pDst, int N);