#include "utils.h"

// checksum
int check(float * result, float * ground_truth, int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
      if(round(result[i] * TOL) != round(ground_truth[i] * TOL)) {
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
  
  float norm_2, temp;
  
  for(int i = 0; i < n; i++) {
      temp = vect[i]; 
      norm_2 += temp * temp;
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
void gemv(float* mat_i, float* vec_1, int size_N, int size_M, float* vec_o) {
    int i, j;
    float temp, temp_1, temp_2, temp_3;
    for (i = 0; i < size_N; i += 4){
      temp = 0.0f;
      temp_1 = 0.0f;
      temp_2 = 0.0f;
      temp_3 = 0.0f;
      for (j = 0; j < size_M; j++){
          float shared = vec_1[j];

          temp += mat_i[i*size_M+j] * shared;
          temp_1 += mat_i[(i+1)*size_M+j] * shared;
          temp_2 += mat_i[(i+2)*size_M+j] * shared;
          temp_3 += mat_i[(i+3)*size_M+j] * shared;
      }
      vec_o[i] = temp;
      vec_o[i+1] = temp_1;
      vec_o[i+2] = temp_2;
      vec_o[i+3] = temp_3;
    }
}

// vectorial product
void outer(float* vec_1, float* vec_2, int size_N, int size_M, float* matrix_o) {
    int i,j;
    float temp, temp_1, temp_2, temp_3;
    for(i = 0; i < size_N; i += 2){
      for(j = 0; j < size_M; j++){
          // be ware: need a 0.0 temp var to make correct vectorial product
          temp = 0.0f;
          temp_1 = 0.0f;
          temp_2 = 0.0f;
          temp_3 = 0.0f;

          float shared = vec_2[j];

          temp += vec_1[i] * shared;
          temp_1 += vec_1[i+1] * shared;
          temp_2 += vec_1[i+2] * shared;
          temp_3 += vec_1[i+3] * shared;
          
          matrix_o[i*size_N + j] = temp;
          matrix_o[(i+1)*size_N + j] = temp_1;
          matrix_o[(i+2)*size_N + j] = temp_2;
          matrix_o[(i+3)*size_N + j] = temp_3;
      }
    }
}