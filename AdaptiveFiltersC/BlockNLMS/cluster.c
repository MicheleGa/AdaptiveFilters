#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"


// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[FILTER_X_SIZE];
  float block[BLOCK_SIZE];
  float filter_d;

  #ifdef DEBUG
  float error[N_SAMPLES];
  #endif
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
float w_L2[LENGTH] = W_NLMS_INIT;
// X is a known driving signal
float x_L2[N_SAMPLES] = NLMS_X;
// input is the input signal with some noise added
float input_L2[N_SAMPLES] = NLMS_INPUT;
// NLMS final filter parameters
float filter_w_check_L2[LENGTH] = FINAL_NLMS_FILTER_W;

#ifdef DEBUG
// interesting to see all the history to appreciate steady-state
float error_check_L2[N_SAMPLES] = NLMS_ERROR;
#endif

#ifdef DEBUG
PI_L1 float diff[LENGTH];
#endif

// why using iteration insteand directly the index of update's for -> modulo opration inside update problematic
int iteration = 0;

void print_array(float * arr, int n) {
  printf("[");
  for (int i = 0; i < (n - 1); i++) {
      printf("%f, ", arr[i]);
    }
  printf("%f]\n", arr[n - 1]);
}

void mat_transpose(float * mat_in, float * mat_out, int sizeM, int sizeN){
  for (int i = 0; i < sizeM; i++) {
    for (int j = 0; j < sizeN; j++) {
      mat_out[i*sizeN+j] = mat_in[j*sizeM+i];
    }
  }
}

float norm_L2(float * vect, int n) {
  
  float norm_2 = 0.0f;
  float temp;
  for(int i = 0; i < n; i++) {
      temp = vect[i];
      norm_2 += temp * temp;
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

void adaptive_filters_nlms() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i]);

      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = nlms.filter_w[j] - input_data.w[j];
      
      nlms.error[i] = norm_L2(diff, LENGTH);
      #endif
    
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

    // init filter's local x, w, block and d
    zeros(nlms.filter_x, FILTER_X_SIZE);
    
    zeros(nlms.filter_w, LENGTH);

    zeros(nlms.block, BLOCK_SIZE);
    
    nlms.filter_d = 0.0f;

    #ifdef DEBUG
    // init with zeros: error, filter x and w, 
    zeros(nlms.error, N_SAMPLES);
    #endif

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

  adaptive_filters_nlms();

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  PRINT_STATS();

  #ifdef DEBUG
  // check the result
  printf("Final error: %f\n", nlms.error[N_SAMPLES - 1]);
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