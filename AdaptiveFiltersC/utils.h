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