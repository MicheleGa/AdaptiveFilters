#include "pmsis.h"
#include "plp_math.h"
#include "math.h"
#include "data.h"
#include "perf.h"
#include "utils.h"

// algorithm specific constants
#define RLS_LMBD 1.0f
#define RLS_LMBD_INV 1.0f / RLS_LMBD
#define RLS_DELTA 2.0f

// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[LENGTH]; 
  float filter_d;

  float g[LENGTH];
  float P[LENGTH * LENGTH];
  float aux[LENGTH];
  // try to make out_buff a local var of fun update()
  float outer_buff[LENGTH * LENGTH];
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
float w_L2[LENGTH] = W_INIT;
// X is a known driving signal
float x_L2[N_SAMPLES] = X;
// input is the input signal with some noise added
float input_L2[N_SAMPLES] = INPUT;
// RLS final filter parameters
float filter_w_check_L2[LENGTH] = W_RLS_FILTER_FINAL;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = RLS_ERROR;

PI_L1 float error[N_SAMPLES];
PI_L1 float diff[LENGTH];
#endif

void __attribute__((noinline)) update(float x_n, float d_n) {

  int i; 
  float acc = 0.0f, acc_1 = 0.0f;

  // shift elements in array on the right (last elemnt thrown away)
  for(i = (LENGTH-1); i > 0; i--) {
    rls.filter_x[i] = rls.filter_x[i-1];
  }
  rls.filter_x[0] = x_n;
  
  // calculate error
  for(i = 0; i < LENGTH; i++) {
    acc += rls.filter_x[i] * rls.filter_w[i];
  }

  acc = d_n - acc;

  // compute gain vector
  for(i = 0; i < LENGTH; i++) {
    rls.aux[i] = rls.filter_x[i] * RLS_LMBD_INV;
  }

  gemv(rls.P, rls.aux, LENGTH, LENGTH, rls.g);
    
  for(i = 0; i < LENGTH; i++) {
    acc_1 += rls.filter_x[i] * rls.g[i];
  }

  acc_1++;

  for(i = 0; i < LENGTH; i++) {
    rls.g[i] /= acc_1;
  }

  // update filter's w
  for(i = 0; i < LENGTH; i++) {
    rls.filter_w[i] += acc * rls.g[i];
  }

  // update P (reuse aux)
  gemv(rls.P, rls.filter_x, LENGTH, LENGTH, rls.aux);
  
  // vectorial product
  outer(rls.g, rls.aux, LENGTH, LENGTH, rls.outer_buff);
  
  // element-wise subtraction and multiplication
  for(i = 0; i < LENGTH; i++) {
    for(int j = 0; j < LENGTH; j++) {
      rls.P[i * LENGTH + j] -= rls.outer_buff[i * LENGTH + j];
      rls.P[i * LENGTH + j] *= RLS_LMBD_INV;
    }
  }
}

void __attribute__((noinline)) adaptive_filters_rls() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i]);
      
      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = rls.filter_w[j] - input_data.w[j];
      
      error[i] = norm_L2(diff, LENGTH);
      #endif      
    }
}

void init() {

    int i;
    // move data from L2 to L1
    for(i = 0; i < LENGTH; i++) {
      input_data.w[i] = w_L2[i];
      input_data.filter_w_check[i] = filter_w_check_L2[i];
    }
    for(i = 0; i < N_SAMPLES; i++) {
      input_data.x[i] = x_L2[i];
      input_data.input[i] = input_L2[i];

      #ifdef DEBUG
      input_data.error_check[i] = error_check_L2[i];
      #endif
    }

    // normalize w
    float norm_2 = norm_L2(input_data.w, LENGTH);
    for(i = 0; i < LENGTH; i++) {
      input_data.w[i] /= norm_2;
    }
    
    // init filter's local x, w and d
    zeros(rls.filter_x, LENGTH);

    zeros(rls.filter_w, LENGTH);

    rls.filter_d = 0.0f;

    #ifdef DEBUG
    // init with zeros: error, filter x and w, 
    zeros(error, N_SAMPLES);
    #endif

    if (RLS_DELTA <= 0.0f) {
      printf("Delta must be a positive number ...");
      pmsis_exit(-1);
    }
    
    // make a diagonal matrix with elements 1 / delta in the diagonal, i.e. P
    eye(1 / RLS_DELTA, rls.P, LENGTH);

    // init outer buffer with zeros
    for(i = 0; i < LENGTH; i++) {
      for(int j = 0; j < LENGTH; j++) {
        rls.outer_buff[i * LENGTH + j] = 0.0f;
      }
    }

    // initial g
    zeros(rls.g, LENGTH);    
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

  if(pi_core_id() == 0) {
    #ifdef DEBUG
    // check the result
    printf("Final error: %f\n", error[N_SAMPLES - 1]);
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
  }
  pi_cl_team_barrier();

  pmsis_exit(0);
}
