#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"


// TODO: make structures (Struct) instead of this shit with variables everywhere

// unknown filter
float w_L2[LENGTH] = W_NLMS_INIT;
PI_L1 float w[LENGTH];

// X is a known driving signal
float x_L2[N_SAMPLES] = NLMS_X;
PI_L1 float x[N_SAMPLES] = NLMS_X;

// input is the input signal with some noise added
float input_L2[N_SAMPLES] = NLMS_INPUT;
PI_L1 float input[N_SAMPLES];

float error[N_SAMPLES];
float error_check[N_SAMPLES] = NLMS_ERROR;

// NLMS filter parameters
float filter_w_check_L2[LENGTH] = FINAL_NLMS_FILTER_W;
PI_L1 float filter_w_check[LENGTH];

PI_L1 float filter_w[LENGTH];
PI_L1 float filter_x[LENGTH]; 
PI_L1 float filter_d;
PI_L1 float diff[LENGTH];

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
    filter_x[i] = filter_x[i-1];
  }
  filter_x[0] = x_n;

  // inner product filter_x and filter_w
  // and between fitler_x and itself
  for(i = 0; i < n; i++) {
    acc += filter_x[i] * filter_w[i];
    acc_1 += filter_x[i] * filter_x[i];
  }
  acc = NLMS_MU * (d_n - acc);

  for(i = 0; i < n; i++) {
    filter_w[i] += acc * (filter_x[i] / acc_1);
  }
}
void adaptive_filters_nlms() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(x[i], input[i], LENGTH);

      #ifdef DEBUG
      // get error
      for(int j = 0; j < LENGTH; j++) 
        diff[j] = filter_w[j] - w[j];
      
      error[i] = norm_L2(diff, LENGTH);
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
      w[i] = w_L2[i];
      x[i] = x_L2[i];
      filter_w_check[i] = filter_w_check_L2[i];
    }
    for(int i = 0; i < N_SAMPLES; i++) {
      input[i] = input_L2[i];
    }
    filter_d = 0.0f;

    // normalize w
    float norm_2 = norm_L2(w, LENGTH);
    for(int i = 0; i < LENGTH; i++) {
      w[i] /= norm_2;
    }

    // init with zeros: error, filter x and w, 
    zeros(error, N_SAMPLES);

    // init filter's local x and w
    zeros(filter_x, LENGTH);

    zeros(filter_w, LENGTH);

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
  printf("Final error: %f\n", error[N_SAMPLES - 1]);
  printf("Ground truth error: %f\n", error_check[N_SAMPLES - 1]);
  #endif

  printf("Final filter w:\n");
  print_array(filter_w, LENGTH);
  printf("Ground truth filter w:\n");
  print_array(filter_w_check, LENGTH);
  errors = check(filter_w, filter_w_check, LENGTH);
  if(errors > 0) 
    printf("You got %d errors on final filter w array, with tollerance %d.\n", errors, TOL);
  errors = 0;

  pmsis_exit(0);
}
