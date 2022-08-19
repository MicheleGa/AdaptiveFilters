#include "pmsis.h"
#include "plp_math.h"
#include "data.h"
#include "perf.h"
#include "utils.h"

// algorithm specific constants
#define NLMS_MU 0.5f

// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[LENGTH]; 
  float filter_d;
} nlms;

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

// unknown filter
float w_L2[LENGTH] = W_INIT;
// X is a known driving signal
float x_L2[N_SAMPLES] = X;
// input is the input signal with some noise added
float input_L2[N_SAMPLES] = INPUT;
// NLMS final filter parameters
float filter_w_check_L2[LENGTH] = W_NLMS_FILTER_FINAL;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = NLMS_ERROR;

// error calculated at each iteration
PI_L1 float error[N_SAMPLES];
PI_L1 float diff[LENGTH];
#endif

void __attribute__((noinline)) update(float x_n, float d_n) {
  
  int i; 
  float acc = 0.0f, acc_1 = 0.0f;
  
  // shift elements in array on the right (last elemnt thrown away)
  for(i = (LENGTH - 1); i > 0; i--) {
    nlms.filter_x[i] = nlms.filter_x[i-1];
  }
  nlms.filter_x[0] = x_n;

  // inner product filter_x and filter_w
  for(i = 0; i < LENGTH; i++) {
    acc += nlms.filter_x[i] * nlms.filter_w[i];
  }

  // inner product between filter_x and itself
  for(i = 0; i < LENGTH; i++) {
    acc_1 += nlms.filter_x[i] * nlms.filter_x[i];
  }
  
  acc = NLMS_MU * (d_n - acc);

  float val = acc / acc_1;

  // update coefficients
  for(i = 0; i < LENGTH; i++) {
    nlms.filter_w[i] += val * nlms.filter_x[i];
  }
}

void __attribute__((noinline)) adaptive_filters_nlms() {
      
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i]);

      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = nlms.filter_w[j] - input_data.w[j];
      
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
    zeros(nlms.filter_x, LENGTH);

    zeros(nlms.filter_w, LENGTH);

    nlms.filter_d = 0.0f;

    #ifdef DEBUG
    zeros(error, N_SAMPLES);
    #endif
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
  adaptive_filters_nlms();

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
    print_array(nlms.filter_w, LENGTH);
    printf("Ground truth filter w:\n");
    print_array(input_data.filter_w_check, LENGTH);

    // checksum
    int errors_counter = check(nlms.filter_w, input_data.filter_w_check, LENGTH);
    if(errors_counter > 0) 
      printf("You got %d errors on final filter w array, with tollerance %d.\n", errors_counter, TOL);
    errors_counter = 0;
  }
  pi_cl_team_barrier();

  pmsis_exit(0);
}
