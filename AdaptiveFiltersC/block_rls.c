#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"
#include "utils.h"


// algorithm specific constants
// algorithm specific constants
#define RLS_LMBD 1.0f
#define RLS_LMBD_INV 1.0f / RLS_LMBD
#define RLS_DELTA 2.0f
#define BLOCK_SIZE 8
#define FILTER_X_SIZE (BLOCK_SIZE + LENGTH - 1)


// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[FILTER_X_SIZE];
  float block[BLOCK_SIZE];
  float filter_d;

  float g[LENGTH];
  float P[LENGTH * LENGTH];
  // try to make out_buff a local var of fun update()
  float outer_buff[LENGTH * LENGTH];
} block_rls;

// auxiliary data for block lms management
PI_L1 struct AuxData{

  // N.B.: lmbd_pwr/lmbd_inv_pwr have fixed-length (original version is variable on demand)
  float lmbd_pwr[2 * BLOCK_SIZE];   // pre compute lambda powers for efficiency 
  float lmbd_inv_pwr[2 * BLOCK_SIZE];
  float H[BLOCK_SIZE * LENGTH];   // hankel matrix
  float H_t[LENGTH * BLOCK_SIZE];  // just H transposed
  float pi[LENGTH * BLOCK_SIZE];    // aux matrix for intermediate computations
  float pi_t[BLOCK_SIZE * LENGTH];    // just pi transposed
  float aux_1[BLOCK_SIZE * BLOCK_SIZE];    // store H * pi
  float diag[BLOCK_SIZE * BLOCK_SIZE];    // store H * pi
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
float filter_w_check_L2[LENGTH] = W_BLOCK_RLS_FILTER_FINAL;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = BLOCK_RLS_ERROR;

// error calculated at each iteration
PI_L1 float error[N_SAMPLES];
PI_L1 float diff[LENGTH];
#endif

// why using iteration instead of directly the index of update's for 
// -> modulo operation inside update is problematic
int iteration = 0;

void update(float x_n, float d_n) {
  
  iteration++;
  int slot = BLOCK_SIZE - ((iteration - 1) % BLOCK_SIZE) - 1;
  
  block_rls.filter_x[slot] = x_n;
  block_rls.block[slot] = d_n;

  // perform an update of the coefficients every BLOCK_SIZE samples
  if(iteration % BLOCK_SIZE == 0) {
    
    int i,j; 
    float acc, acc1;
  
    // build hankel matrix: the hankel matrix has constant anti-diagonals, 
    // with c as its first column (first BLOCK_SIZE elements from block_rls.filter_x) and 
    // r as its last row (last LENGTH elements from block_rls.filter_x)
    for(i = 0; i < BLOCK_SIZE; i++) {
      for(j = 0; j < (BLOCK_SIZE - i); j++) {
        aux_data.H[i*LENGTH+j] = block_rls.filter_x[j+i];
      }
      for(int k = 0; k < (LENGTH - j); k++) {
        aux_data.H[i*LENGTH+j+k] = block_rls.filter_x[BLOCK_SIZE+k];
      }
    }
    
    // calculate error
    gemv(BLOCK_SIZE, LENGTH, aux_data.H, block_rls.filter_w, aux_data.err);
    
    for(i = 0; i < BLOCK_SIZE; i++) {
      aux_data.err[i] = block_rls.block[i] - aux_data.err[i]; 
    }

    // transpose 
    mat_transpose(aux_data.H, aux_data.H_t, LENGTH, BLOCK_SIZE);

    // calculate gain vector
    matMul(block_rls.P, aux_data.H_t, aux_data.pi, LENGTH, LENGTH, BLOCK_SIZE);

    // transpose H
    mat_transpose(aux_data.H, aux_data.H_t, LENGTH, BLOCK_SIZE);

    // transpose pi
    mat_transpose(aux_data.pi, aux_data.pi_t, BLOCK_SIZE, LENGTH);

    // H * pi
    matMul(aux_data.H, aux_data.pi, aux_data.aux_1, BLOCK_SIZE, LENGTH, BLOCK_SIZE);

    // diag + H * pi
    matAdd(aux_data.diag, aux_data.aux_1, aux_data.aux_1, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    mat_transpose(aux_data.aux_1, aux_data.aux_1, BLOCK_SIZE, BLOCK_SIZE);

    // (diag + H * pi).T -> coefficient matrix
    // pi.T -> dependent variables matrix
    print_array_2d(aux_data.aux_1, BLOCK_SIZE, BLOCK_SIZE);
  }
}

void adaptive_filters_block_rls() {
    for(int i = 0; i < 10; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i]);

      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = block_rls.filter_w[j] - input_data.w[j];
      
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
    zeros(block_rls.filter_x, LENGTH);

    zeros(block_rls.filter_w, LENGTH);

    zeros(block_rls.block, BLOCK_SIZE);

    block_rls.filter_d = 0.0f;

    if (RLS_DELTA <= 0.0f) {
      printf("Delta must be a positive number ...");
      pmsis_exit(-1);
    }
    
    // make a diagonal matrix with elements 1 / delta in the diagonal, i.e. P
    eye(1 / RLS_DELTA, block_rls.P, LENGTH);

    // init outer buffer with zeros
    for(i = 0; i < LENGTH; i++) {
      for(int j = 0; j < LENGTH; j++) {
        block_rls.outer_buff[i * LENGTH + j] = 0.0f;
      }
    }

    // initial g
    zeros(block_rls.g, LENGTH);

    // pre-compute powers of lambda/lambda inv
    aux_data.lmbd_pwr[0] = 1.0f;
    aux_data.lmbd_inv_pwr[0] = 1.0f;
    for(i = 1; i < 2 * BLOCK_SIZE; i++) {
      aux_data.lmbd_pwr[i] = aux_data.lmbd_pwr[i - 1] * RLS_LMBD;
      aux_data.lmbd_inv_pwr[i] = aux_data.lmbd_inv_pwr[i - 1] * RLS_LMBD_INV;
    }

    // pre-compute diagonal matrix that will be used later in the update function
    for(i = 0; i < BLOCK_SIZE; i++) {
      for(int j = 0; j < BLOCK_SIZE; j++) {
        if (i == j) {
          aux_data.diag[i*BLOCK_SIZE+j] = aux_data.lmbd_pwr[BLOCK_SIZE - i];
        }
        else {
          aux_data.diag[i*BLOCK_SIZE+j] = 0.0f;
        }
      }
    }

    print_array_2d(aux_data.diag, BLOCK_SIZE, BLOCK_SIZE);   

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
  adaptive_filters_block_rls();

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
  print_array(block_rls.filter_w, LENGTH);
  printf("Ground truth filter w:\n");
  print_array(input_data.filter_w_check, LENGTH);

  // checksum
  int errors_counter = check(block_rls.filter_w, input_data.filter_w_check, LENGTH);
  if(errors_counter > 0) 
    printf("You got %d errors on final filter w array, with tollerance %d.\n", errors_counter, TOL);
  errors_counter = 0;

  pmsis_exit(0);
}