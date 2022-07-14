#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"


// adaptive filter data structures
PI_L1 struct AdaptiveFilter{

  float filter_w[LENGTH];
  float filter_x[LENGTH]; 
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
/*
float max_val(float * f, int n) {
  float m = f[0];
  for(int i = 1; i < n; i++) {
    if(f[i] > m)
      m = f[i];
  }

  return m;
}

void quantize() {
  int N = 3 * 3;
  float f[] = {0.34, 3.75, 5.64, 1.12, 2.7, -0.9, -4.7, 0.68, 1.43};

  // from float to signed int 8
  int B = 8;

  // signed quantization
  float aux = (pow(2, B - 1) - 1) / max_val(f, N);
  aux = log(aux)/log(2);
  aux = (float) floor(aux); // number of integer bits
  
  char fractionary_bits = B - ((int) aux);
  
  char res[N]; 
  int var = 0;
  for(int i = 0; i < N; i++) {
    var = (int) floor(f[i] * (1 << fractionary_bits));
    if(var < 0) {
      var = var > 0 ? var : -var;
      var = ~var;
      var++;
    }
    printf("%d\n", var);
  }
}*/

void update(float x_n, float d_n, int n) {
  int i; 
  float acc = 0.0f, acc_1 = 0.0f;
  
  // shift elements in array on the right (last elemnt thrown away)
  for(i = (n-1); i > 0; i--) {
    nlms.filter_x[i] = nlms.filter_x[i-1];
  }
  nlms.filter_x[0] = x_n;

  // inner product filter_x and filter_w
  // and between fitler_x and itself
  for(i = 0; i < n; i++) {
    acc += nlms.filter_x[i] * nlms.filter_w[i];
    acc_1 += nlms.filter_x[i] * nlms.filter_x[i];
  }
  acc = NLMS_MU * (d_n - acc);

  for(i = 0; i < n; i++) {
    nlms.filter_w[i] += acc * (nlms.filter_x[i] / acc_1);
  }
}

void adaptive_filters_nlms() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(input_data.x[i], input_data.input[i], LENGTH);

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

    // init filter's local x, w and d
    zeros(nlms.filter_x, LENGTH);

    zeros(nlms.filter_w, LENGTH);

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
