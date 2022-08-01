#include "utils.h"

// checksum
int check(float * result, float * ground_truth, int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
      if(round(result[i]* TOL) != round(ground_truth[i]*TOL)) {
          count++;
          printf("Error at position %d, got %f instead of %f\n", i, result[i], ground_truth[i]);
      }
    }
    return count;
}

// just print a 1-d array
void print_array(float * arr, int n) {
  printf("[");
  for (int i = 0; i < (n - 1); i++) {
      printf("%f, ", arr[i]);
    }
  printf("%f]\n", arr[n - 1]);
}

// just print a 2-d array
void print_array_2d(float * arr, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        printf("%f ", arr[i*m+j]);
    }
    printf("\n");    
  }
}

// calculate norm-2 (scalar)
float norm_L2(float * vect, int n) {
  
  float norm_2 = 0.0f;
  
  for(int i = 0; i < n; i++) {
      norm_2 += vect[i] * vect[i];
  }

  return sqrt(norm_2);
}

// just a numpy-zeros-like function
void zeros(float * arr, int n) {
  for(int i = 0; i < n; i++) {
    arr[i] = 0.0f;
  }
}

// init diagonal matrix with element in the diagonal
void eye(float element, float * matrix, int n) {
  int pos;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      pos = i * n + j;
      if(i == j) 
        matrix[pos] = element;
      else
        matrix[pos] = 0.0f;
    }
  }
}

// matrix-vector product
void gemv(int size_N, int size_M, float* mat_i, float* vec_1, float* vec_o) {
    for (int i=0; i<size_N; i++){
      float temp = 0.0f;
      for (int j=0; j<size_M; j++){
          temp += mat_i[i*size_M+j] * vec_1[j];
      }
      vec_o[i] = temp;
    }
}

// vectorial product
void outer(int size_N, int size_M, float* vec_1, float* vec_2, float* matrix_o) {
    for (int i=0; i<size_N; i++){
      for (int j=0; j<size_M; j++){
          matrix_o[i*size_N + j] = 0.0f;
          matrix_o[i*size_N + j] += vec_1[i] * vec_2[j];
      }
    }
}

// transpose matrix
void mat_transpose(float * mat_in, float * mat_out, int sizeM, int sizeN) {
  for (int i = 0; i < sizeM; i++) {
    for (int j = 0; j < sizeN; j++) {
      mat_out[i*sizeN+j] = mat_in[j*sizeM+i];
    }
  }
}
