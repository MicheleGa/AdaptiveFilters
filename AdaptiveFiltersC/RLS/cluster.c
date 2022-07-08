#include "pmsis.h"
#include "math.h"
#include "data.h"
#include "perf.h"


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
  acc = d_n - acc;

  for(i = 0; i < n; i++) {
    filter_w[i] += NLMS_MU * acc * (filter_x[i] / acc_1);
  }
}

void adaptive_filters_nlms() {
    for(int i = 0; i < N_SAMPLES; i++) {
      // update filter_x, then d, and eventually filter_w
      update(x[i], input[i], LENGTH);

      // get error
      for(int j = 0; j < LENGTH; j++) {
        // clean prev result in diff
        diff[j] = 0.0f;
        diff[j] = filter_w[j] - w[j];
      }
      error[i] = norm_L2(diff, LENGTH);
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

  adaptive_filters_nlms();

  // stop measuring
  STOP_STATS();

  // end of the performance statistics loop
  PRINT_STATS();

  // check the result
  printf("Final error:\n");
  print_array(error, N_SAMPLES);
  printf("Ground truth error:\n");
  print_array(error_check, N_SAMPLES);
  errors = check(error, error_check, N_SAMPLES);
  if(errors > 0) 
    printf("You got %d errors on final error array.\n", errors);
  errors = 0;

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
