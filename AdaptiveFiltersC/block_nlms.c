#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"
#include "utils.h"


// algorithm specific constants
#define NLMS_MU 0.5f
#define BLOCK_SIZE 2
#define FILTER_X_SIZE (BLOCK_SIZE + LENGTH - 1)


// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[FILTER_X_SIZE];
  float block[BLOCK_SIZE];
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

// data from data.h
// unknown filter
float w_L2[LENGTH] = W_INIT;
// X is a known driving signal
float x_L2[N_SAMPLES] = X;
// input is the input signal with some noise added
float input_L2[N_SAMPLES] = INPUT;
// NLMS final filter parameters
float filter_w_check_L2[LENGTH] = FINAL_FILTER_W;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = ERROR;

PI_L1 float error[N_SAMPLES];
PI_L1 float diff[LENGTH];
#endif

// why using iteration instead of directly the index of update's for 
// -> modulo operation inside update is problematic
int iteration = 0;

void update(float x_n, float d_n) {
  
  iteration++;
  int slot = BLOCK_SIZE - ((iteration - 1) % BLOCK_SIZE) - 1;
  
  nlms.filter_x[slot] = x_n;
  nlms.block[slot] = d_n;

  // perform an update of the coefficients every BLOCK_SIZE samples
  if(iteration % BLOCK_SIZE == 0) {
    
    int i; 
    float acc = 0.0f, acc_1 = 0.0f;
  
    // build hankel matrix: the hankel matrix has constant anti-diagonals, 
    // with c as its first column (first BLOCK_SIZE elements from nlms.filter_x) and 
    // r as its last row (last LENGTH elements from nlms.filter_x)
    float H[BLOCK_SIZE * LENGTH];

    for(i = 0; i < BLOCK_SIZE; i++) {
      int j;
      for(j = 0; j < (BLOCK_SIZE - i); j++) {
        H[i*LENGTH+j] = nlms.filter_x[j+i];
      }
      for(int k = 0; k < (LENGTH - j); k++) {
        H[i*LENGTH+j+k] = nlms.filter_x[BLOCK_SIZE+k];
      }
    }
    
    // calculate error
    float err[BLOCK_SIZE];
    gemv(BLOCK_SIZE, LENGTH, H, nlms.filter_w, err);
    
    for(i=0; i < BLOCK_SIZE; i++) {
      err[i] = nlms.block[i] - err[i]; 
    }

    // calculate H norm along axis 1
    float norm[BLOCK_SIZE];
    float temp;
    for(i=0; i < BLOCK_SIZE; i++) {
      for(int j=0; j < LENGTH; j++) {
        temp = H[i*LENGTH+j];
        norm[i] += temp * temp;
      }
    }
    
    // transpose e divide by norm
    float H_t[LENGTH * BLOCK_SIZE];
    mat_transpose(H, H_t, LENGTH, BLOCK_SIZE);

    for(i=0; i < LENGTH; i++) {
      for(int j=0; j < BLOCK_SIZE; j++) {
          H_t[i*BLOCK_SIZE+j] /= norm[j];
      }
    }

    // update coefficients
    float aux[LENGTH];
    gemv(LENGTH, BLOCK_SIZE, H_t, err, aux);

    for(i=0; i < LENGTH; i++) {
      nlms.filter_w[i] += NLMS_MU * aux[i];
    }

    // remember a few values
    // need to make a copy of filter_x because we would like to change a data structure while iterating
    // over it
    float temp_filter_x[FILTER_X_SIZE];
    for(i=0; i < FILTER_X_SIZE; i++) {
      temp_filter_x[i] = nlms.filter_x[i]; 
    }
    for(i=0; i < (LENGTH - 1); i++) {
      nlms.filter_x[FILTER_X_SIZE - LENGTH + 1 + i] = temp_filter_x[i]; 
    }
  }
}

void adaptive_filters_block_nlms() {
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

    // init filter's local x, w, block and d
    zeros(nlms.filter_x, FILTER_X_SIZE);
    
    zeros(nlms.filter_w, LENGTH);

    zeros(nlms.block, BLOCK_SIZE);
    
    nlms.filter_d = 0.0f;

    #ifdef DEBUG
    // init with zeros: error, filter x and w, 
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

  pmsis_exit(0);
}