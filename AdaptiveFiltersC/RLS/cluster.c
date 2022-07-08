#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"
#include "buffer.h"


// RLS data structures 
float w[LENGTH] = W_RLS_INIT;
float w_check[LENGTH] = W_RLS_CHECK;
// X is a known driving signal
float x[N_SAMPLES] = RLS_X;
// input is the input signal with some noise added
float input[N_SAMPLES] = RLS_INPUT;
float error[N_SAMPLES];
float error_check[N_SAMPLES] = RLS_ERROR;

float filter_w[LENGTH];
float filter_w_check[LENGTH] = FINAL_RLS_FILTER_W;
float filter_x[LENGTH]; 
float filter_d = 0.0f;
float diff[LENGTH];

float P[LENGTH * LENGTH];
float buffer[BUFF_LENGTH];
// buffer can be doubled
//int current_buff_len = LENGTH;
int free_space = BUFF_LENGTH;
float outer_buff[LENGTH * LENGTH];
float g[LENGTH];

// errors counter
int errors = 0;


void print_array(float * arr, int n) {
  printf("[");
  for (int i = 0; i < (n - 1); i++) {
      printf("%f, ", arr[i]);
    }
  printf("%f]\n", arr[n - 1]);
}

float norm_L2(float * vect, int n) {
  
  int i;
  float norm_2 = 0.0f;
  
  for(i = 0; i < n; i++) {
      norm_2 += vect[i] * vect[i];
  }

  return sqrt(norm_2);
}

void gemv(int size_N, int size_M, float* mat_i, float* vec_1, float* vec_o){
    for (int i=0; i<size_N; i++){
      float temp = 0.0f;
      for (int j=0; j<size_M; j++){
          temp += mat_i[i*size_M+j] * vec_1[j];
      }
      vec_o[i] = temp;
    }
}

void outer(int size_N, int size_M, float* vec_1, float* vec_2, float* matrix_o){
    for (int i=0; i<size_N; i++){
      for (int j=0; j<size_M; j++){
          // multiply accumulate operation (MAC)
          matrix_o[i*size_N + j] += vec_1[i] * vec_2[j];
      }
    }
}

void update(float x_n, float d_n, int n) {
  int i; 
  float acc = 0.0f, acc_1 = 0.0f;

  // push on the buffer
  free_space--;
  buffer[free_space] = x_n;

  // read LENGTH elements from the buffer
  for (i = 0; i < LENGTH; i++) {
    filter_x[i] = buffer[free_space + i];
  }
  
  for(i = 0; i < LENGTH; i++) {
    acc += filter_x[i] * filter_w[i];
  }

  acc = d_n - acc;

  float aux[LENGTH];

  for(i = 0; i < LENGTH; i++) {
    aux[i] = filter_x[i] * RLS_LMBD_INV;
  }

  gemv(LENGTH, LENGTH, P, aux, g);
    
  for(i = 0; i < LENGTH; i++) {
    acc_1 += filter_x[i] * g[i];
  }

  acc_1++;

  for(i = 0; i < LENGTH; i++) {
    g[i] /= acc_1;
  }

  for(i = 0; i < LENGTH; i++) {
    filter_w[i] += acc * g[i];
  }

  float partial[LENGTH];
  gemv(LENGTH, LENGTH, P, filter_x, partial);
  
  outer(LENGTH, LENGTH, g, partial, outer_buff);

  for(i = 0; i < LENGTH; i++) {
    for(int j = 0; j < LENGTH; j++) {
      P[i * LENGTH + j] -= outer_buff[i * LENGTH + j];
      P[i * LENGTH + j] *= RLS_LMBD_INV;
    }
  }
}

void adaptive_filters_rls() {
    for(int i = 0; i < 10; i++) {
      // update filter_x, then d, and eventually filter_w
      update(x[i], input[i], LENGTH);
      printf("w at iteration %d\n", i);
      print_array(filter_w, LENGTH);
      // flush adjust value when buffer wil be dynamically resized
      if (i % 160 == 0 && i > 0) {
        
        int num = BUFF_LENGTH - free_space;
        free_space += num;

        // shift elements in array on the right 
        for(int i = (BUFF_LENGTH - num - 1); i > 0; i--) 
            buffer[i + num] = buffer[i];
      }
      // get error
      for(int j = 0; j < LENGTH; j++) {
        // clean prev result in diff
        diff[j] = 0.0f;
        diff[j] = filter_w[j] - w[j];
      }
      error[i] = norm_L2(diff, LENGTH);
      
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

void zeros(float * arr, int n) {
  for(int i = 0; i < n; i++) {
    arr[i] = 0.0f;
  }
}

void init() {

    // print initial value of the filter
    printf("Initial filter value w:\n");
    print_array(w, LENGTH);

    // normalize w
    float norm_2 = norm_L2(w, LENGTH);
    for(int i = 0; i < LENGTH; i++) {
      w[i] /= norm_2;
    }

    printf("Normalized w:\n");
    print_array(w, LENGTH);

    // init error with zeros (may unseful since 0.0 is the default value for C)
    zeros(error, N_SAMPLES);

    //printf("Init error:\n");
    //print_array(error, N_SAMPLES);

    // init filter's local x and w
    zeros(filter_x, LENGTH);

    //printf("Init local filter x:\n");
    //print_array(filter_x, LENGTH);

    zeros(filter_w, LENGTH);

    //printf("Init local filter w:\n");
    //print_array(filter_w, LENGTH);

    if (RLS_DELTA <= 0.0f) {
      printf("Delta must be a positive number ...");
      pmsis_exit(-1);
    }
    
    // make a diagonal matrix with elements 1 / delta in the diagonal, i.e. P
    eye(1 / RLS_DELTA, P, LENGTH);

    // intial buffer with zeros
    zeros(buffer, LENGTH);

    // N.B. init buffer with (LENGTH - 1) 0s 
    free_space -= (LENGTH - 1);

    // init outer buffer with zeros
    for(int i = 0; i < LENGTH; i++) {
      for(int j = 0; j < LENGTH; j++) {
        outer_buff[i * LENGTH + j] = 0.0f;
      }
    }

    // initial g
    zeros(g, LENGTH);    
}

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

void cluster_fn() {

  // init performance counters
  INIT_STATS();
  
  // set initial values (not considered by performance counters)
  init();
  errors = check(w, w_check, LENGTH);
  if(errors > 0) 
    printf("You got %d errors on w normalization.", errors);
  errors = 0;

  // reset stats and start measuring 
  RESET_STATS();
  START_STATS();

  // workload

  adaptive_filters_rls();

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  PRINT_STATS();

  // check the result
  printf("Final error: %f\n", error[N_SAMPLES - 1]);
  printf("Ground truth error: %f\n", error_check[N_SAMPLES - 1]);

  printf("Final filter w:\n");
  print_array(filter_w, LENGTH);
  printf("Ground truth filter w:\n");
  print_array(filter_w_check, LENGTH);
  errors = check(filter_w, w_check, LENGTH);
  if(errors > 0) 
    printf("You got %d errors on final filter w array.\n", errors);
  errors = 0;

  pmsis_exit(0);
}
