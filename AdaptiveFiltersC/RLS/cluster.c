#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"

// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[LENGTH]; 
  float filter_d;

  float g[LENGTH];
  float P[LENGTH * LENGTH];
  // try to make out_buff a local var of fun update()
  float outer_buff[LENGTH * LENGTH];

  #ifdef DEBUG
  float error[N_SAMPLES];
  #endif
} rls;

// input data structures
PI_L1 struct InputData{

  float w[LENGTH]; 
  float x[N_SAMPLES];
  float input[N_SAMPLES];

  // data for checksum
  float filter_w_check[LENGTH];
  
  #ifdef DEBUG
  float error_check[N_SAMPLES];
  #endif
} input_data;

// data from data.h
// unknown filter
float w_L2[LENGTH] = W_RLS_INIT;
// X is a known driving signal
float x_L2[N_SAMPLES] = RLS_X;
// input is the input signal with some noise added
float input_L2[N_SAMPLES] = RLS_INPUT;
// NLMS final filter parameters
float filter_w_check_L2[LENGTH] = FINAL_RLS_FILTER_W;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = RLS_ERROR;
#endif

#ifdef DEBUG
PI_L1 float diff[LENGTH];
#endif

void print_array(float * arr, int n) {
  printf("[");
  for (int i = 0; i < (n - 1); i++) {
      printf("%f, ", arr[i]);
    }
  printf("%f]\n", arr[n - 1]);
}

float norm_L2(float * vect, int n) {
  
  float norm_2 = 0.0f;
  
  for(int i = 0; i < n; i++) {
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
          matrix_o[i*size_N + j] = 0.0f;
          matrix_o[i*size_N + j] += vec_1[i] * vec_2[j];
      }
    }
}

void update(float x_n, float d_n, int n) {
  int i; 
  float acc = 0.0f, acc_1 = 0.0f;

  // shift elements in array on the right (last elemnt thrown away)
  for(i = (n-1); i > 0; i--) {
    rls.filter_x[i] = rls.filter_x[i-1];
  }
  rls.filter_x[0] = x_n;
  
  for(i = 0; i < LENGTH; i++) {
    acc += rls.filter_x[i] * rls.filter_w[i];
  }

  acc = d_n - acc;

  float aux[LENGTH];

  for(i = 0; i < LENGTH; i++) {
    aux[i] = rls.filter_x[i] * RLS_LMBD_INV;
  }

  gemv(LENGTH, LENGTH, rls.P, aux, rls.g);
    
  for(i = 0; i < LENGTH; i++) {
    acc_1 += rls.filter_x[i] * rls.g[i];
  }

  acc_1++;

  for(i = 0; i < LENGTH; i++) {
    rls.g[i] /= acc_1;
  }

  for(i = 0; i < LENGTH; i++) {
    rls.filter_w[i] += acc * rls.g[i];
  }

  float partial[LENGTH];
  gemv(LENGTH, LENGTH, rls.P, rls.filter_x, partial);
  
  outer(LENGTH, LENGTH, rls.g, partial, rls.outer_buff);
  
  for(i = 0; i < LENGTH; i++) {
    for(int j = 0; j < LENGTH; j++) {
      rls.P[i * LENGTH + j] -= rls.outer_buff[i * LENGTH + j];
      rls.P[i * LENGTH + j] *= RLS_LMBD_INV;
    }
  }
}

void adaptive_filters_rls() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i], LENGTH);
      
      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = rls.filter_w[j] - input_data.w[j];
      
      rls.error[i] = norm_L2(diff, LENGTH);
      #endif
      
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

    // move data from L2 to L1
    for(int i = 0; i < LENGTH; i++) {
      input_data.w[i] = w_L2[i];
      input_data.filter_w_check[i] = filter_w_check_L2[i];
    }
    for(int i = 0; i < N_SAMPLES; i++) {
      input_data.x[i] = x_L2[i];
      input_data.input[i] = input_L2[i];

      #ifdef DEBUG
      input_data.error_check[i] = error_check_L2[i];
      #endif
    }

    // normalize w
    float norm_2 = norm_L2(input_data.w, LENGTH);
    for(int i = 0; i < LENGTH; i++) {
      input_data.w[i] /= norm_2;
    }
    
    // init filter's local x, w and d
    zeros(rls.filter_x, LENGTH);

    zeros(rls.filter_w, LENGTH);

    rls.filter_d = 0.0f;

    #ifdef DEBUG
    // init with zeros: error, filter x and w, 
    zeros(rls.error, N_SAMPLES);
    #endif

    if (RLS_DELTA <= 0.0f) {
      printf("Delta must be a positive number ...");
      pmsis_exit(-1);
    }
    
    // make a diagonal matrix with elements 1 / delta in the diagonal, i.e. P
    eye(1 / RLS_DELTA, rls.P, LENGTH);

    // init outer buffer with zeros
    for(int i = 0; i < LENGTH; i++) {
      for(int j = 0; j < LENGTH; j++) {
        rls.outer_buff[i * LENGTH + j] = 0.0f;
      }
    }

    // initial g
    zeros(rls.g, LENGTH);    
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

  // reset stats and start measuring 
  RESET_STATS();
  START_STATS();

  // workload

  adaptive_filters_rls();

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  PRINT_STATS();

  #ifdef DEBUG
  // check the result
  printf("Final error: %f\n", rls.error[N_SAMPLES - 1]);
  printf("Ground truth error: %f\n", input_data.error_check[N_SAMPLES - 1]);
  #endif

  // final filter weights
  printf("Final filter w:\n");
  print_array(rls.filter_w, LENGTH);
  printf("Ground truth filter w:\n");
  print_array(input_data.filter_w_check, LENGTH);

  // checksum
  int errors_counter = check(rls.filter_w, input_data.filter_w_check, LENGTH);
  if(errors_counter > 0) 
    printf("You got %d errors on final filter w array, with tollerance %d.\n", errors_counter, TOL);
  errors_counter = 0;

  pmsis_exit(0);
}
