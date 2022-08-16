#include "pmsis.h"
#include "plp_math.h"
#include "math.h"
#include "data.h"
#include "perf.h"
#include "utils.h"

// algorithm specific constants
#define NLMS_MU 0.001f 
#define BLOCK_SIZE 8
#define FILTER_X_SIZE (BLOCK_SIZE + LENGTH - 1)

// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[FILTER_X_SIZE];
  float block[BLOCK_SIZE];
  float filter_d;
} block_nlms;

// auxiliary data for block lms management
PI_L1 struct AuxData{

  float H[BLOCK_SIZE * LENGTH];   // hankel matrix
  float H_t[LENGTH * BLOCK_SIZE];  // just H transposed
  float norm[BLOCK_SIZE];   // H's norm2 along axis 1
  float aux[LENGTH];    // intermediate computation vector to get final w update
  float err[BLOCK_SIZE];    // estimated error: e(n) = y(n) - H(n)*X(n) 
} aux_data;

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
// BLOCK NLMS final filter parameters
float filter_w_check_L2[LENGTH] = W_BLOCK_NLMS_FILTER_FINAL;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = BLOCK_NLMS_ERROR;

// error calculated at each iteration
PI_L1 float error[N_SAMPLES];
PI_L1 float diff[LENGTH];
#endif

// why using iteration instead of directly the index of update's for 
// -> modulo operation inside update is problematic
int iteration = 0;

void __attribute__((noinline)) update(float x_n, float d_n) {
  
  // buffers update
  iteration++;
  int slot = BLOCK_SIZE - ((iteration - 1) % BLOCK_SIZE) - 1;
  
  block_nlms.filter_x[slot] = x_n;
  block_nlms.block[slot] = d_n;

  // perform an update of the coefficients every BLOCK_SIZE samples
  if(iteration % BLOCK_SIZE == 0) {
    
    int i,j; 
    float acc = 0.0f, acc1 = 0.0f;
  
    // build hankel matrix: the hankel matrix has constant anti-diagonals, 
    // with c as its first column (first BLOCK_SIZE elements from block_nlms.filter_x) and 
    // r as its last row (last LENGTH elements from block_nlms.filter_x)
    for(i = 0; i < BLOCK_SIZE; i++) {
      for(j = 0; j < (BLOCK_SIZE - i); j++) {
        aux_data.H[i * LENGTH+j] = block_nlms.filter_x[j + i];
      }
      for(int k = 0; k < (LENGTH - j); k++) {
        aux_data.H[i * LENGTH + j + k] = block_nlms.filter_x[BLOCK_SIZE + k];
      }
    }
    
    // calculate error
    gemv(aux_data.H, block_nlms.filter_w, BLOCK_SIZE, LENGTH, aux_data.err);
    
    for(i = 0; i < BLOCK_SIZE; i++) {
      aux_data.err[i] = block_nlms.block[i] - aux_data.err[i]; 
    }

    // calculate norm 2 along axis 1 (no final sqrt) for matrix H
    for(i = 0; i < BLOCK_SIZE; i++) {
      acc = 0.0f;
      for(j = 0; j < LENGTH; j++) {
        acc1 = aux_data.H[i * LENGTH + j];
        acc += acc1 * acc1;
      }
      aux_data.norm[i] = acc;
    }
    
    // transpose e divide by norm
    plp_mat_trans_f32(aux_data.H, BLOCK_SIZE, LENGTH, aux_data.H_t);

    for(i = 0; i < LENGTH; i++) {
      for(j = 0; j < BLOCK_SIZE; j++) {
          aux_data.H_t[i * BLOCK_SIZE + j] /= aux_data.norm[j];
      }
    }

    // update coefficients
    gemv(aux_data.H_t, aux_data.err, LENGTH, BLOCK_SIZE, aux_data.aux);

    for(i = 0; i < LENGTH; i++) {
      block_nlms.filter_w[i] += NLMS_MU * aux_data.aux[i];
    }
    
    // remember a few values
    // need to make a copy of filter_x because we would like to change a data structure 
    // while iterating over it
    plp_copy_f32(block_nlms.filter_x, aux_data.aux, (LENGTH - 1));
    
    for(i = 0; i < (LENGTH - 1); i++) {
      block_nlms.filter_x[FILTER_X_SIZE - LENGTH + 1 + i] = aux_data.aux[i]; 
    }
  }
}

void __attribute__((noinline)) adaptive_filters_block_nlms() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i]);

      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = block_nlms.filter_w[j] - input_data.w[j];
      
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

    // init filter's local x, w, block and d
    zeros(block_nlms.filter_x, FILTER_X_SIZE);
    
    zeros(block_nlms.filter_w, LENGTH);

    zeros(block_nlms.block, BLOCK_SIZE);
    
    block_nlms.filter_d = 0.0f;

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
  adaptive_filters_block_nlms();

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
    print_array(block_nlms.filter_w, LENGTH);
    printf("Ground truth filter w:\n");
    print_array(input_data.filter_w_check, LENGTH);

    // checksum
    int errors_counter = check(block_nlms.filter_w, input_data.filter_w_check, LENGTH);
    if(errors_counter > 0) 
      printf("You got %d errors on final filter w array, with tollerance %d.\n", errors_counter, TOL);
    errors_counter = 0;
  }
  pi_cl_team_barrier();

  pmsis_exit(0);
}