#include "utils.h"

// checksum
int check(float * result, float * ground_truth, int n) {
    int count = 0;
    for(int i = 0; i < n; i++) {
      if(round(result[i] * TOL) != round(ground_truth[i] * TOL)) {
          count++;
          printf("Error at position %d, got %f instead of %f\n", i, result[i], ground_truth[i]);
      }
    }
    return count;
}

// just print a 1-d array
void print_array(float * arr, int n) {
  printf("[");
  for (int i = 0; i < (n - 1); i++) {
      printf("%f, ", arr[i]);
    }
  printf("%f]\n", arr[n - 1]);
}

// just print a 2-d array
void print_array_2d(float * arr, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        printf("%f ", arr[i*m+j]);
    }
    printf("\n");    
  }
}

// calculate norm-2 (scalar)
float norm_L2(float * vect, int n) {
  
  float norm_2, temp;
  
  for(int i = 0; i < n; i++) {
      temp = vect[i]; 
      norm_2 += temp * temp;
  }
  
  return sqrt(norm_2);
}

// just a numpy-zeros-like function
void zeros(float * arr, int n) {

  for(int i = 0; i < n; i++) {
    arr[i] = 0.0f;
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

// matrix-vector product
void gemv(float * mat_i, float * vec_1, int size_N, int size_M, float * vec_o) {
    int i, j;
    float temp, temp_1, temp_2, temp_3;

    for (i = 0; i < size_N; i += 4){

      temp = 0.0f;
      temp_1 = 0.0f;
      temp_2 = 0.0f;
      temp_3 = 0.0f;

      for (j = 0; j < size_M; j++){

          float shared = vec_1[j];

          temp += mat_i[i*size_M+j] * shared;
          temp_1 += mat_i[(i+1)*size_M+j] * shared;
          temp_2 += mat_i[(i+2)*size_M+j] * shared;
          temp_3 += mat_i[(i+3)*size_M+j] * shared;
      }
      
      vec_o[i] = temp;
      vec_o[i+1] = temp_1;
      vec_o[i+2] = temp_2;
      vec_o[i+3] = temp_3;
    }
}

// vectorial product
void outer(float* vec_1, float* vec_2, int size_N, int size_M, float* matrix_o) {
    int i,j;
    float temp, temp_1, temp_2, temp_3;
    for(i = 0; i < size_N; i += 4){
      for(j = 0; j < size_M; j++){
          // be ware: need a 0.0 temp var to make correct vectorial product
          temp = 0.0f;
          temp_1 = 0.0f;
          temp_2 = 0.0f;
          temp_3 = 0.0f;

          float shared = vec_2[j];

          temp += vec_1[i] * shared;
          temp_1 += vec_1[i+1] * shared;
          temp_2 += vec_1[i+2] * shared;
          temp_3 += vec_1[i+3] * shared;
          
          matrix_o[i*size_N + j] = temp;
          matrix_o[(i+1)*size_N + j] = temp_1;
          matrix_o[(i+2)*size_N + j] = temp_2;
          matrix_o[(i+3)*size_N + j] = temp_3;
      }
    }
}

// transpose matrix
void mat_transpose(float * mat_in, int sizeN, int sizeM, float * mat_out) {
  
  float temp, temp_1, temp_2, temp_3;

  for (int i = 0; i < sizeM; i+=4) {
    for (int j = 0; j < sizeN; j++) {
      temp = mat_in[j * sizeM + i];
      temp_1 = mat_in[j * sizeM + (i+1)];
      temp_2 = mat_in[j * sizeM + (i+2)];
      temp_3 = mat_in[j * sizeM + (i+3)];
      
      mat_out[i * sizeN + j] = temp;
      mat_out[(i+1) * sizeN + j] = temp_1;
      mat_out[(i+2) * sizeN + j] = temp_2;
      mat_out[(i+3) * sizeN + j] = temp_3;
    }
  }
}

// matrix mutliplication
void mat_mul_f32(float * pSrcA, float  * pSrcB, int m, int n, int o, float * pDstC) {
    int i, j, k;
    float sum, sum_1, sum_2, sum_3;

    for (k = 0; k < o; k += 4) {
        for (i = 0; i < m; i++) {
            sum = 0.0f;
            sum_1 = 0.0f;
            sum_2 = 0.0f;
            sum_3 = 0.0f;

            for (j = 0; j < n; j++) {
                
                float shared = pSrcA[i * n + j];
                
                float a = pSrcB[j * o + k];
                float b = pSrcB[j * o + (k+1)];
                float c = pSrcB[j * o + (k+2)];
                float d = pSrcB[j * o + (k+3)];

                sum += a * shared;
                sum_1 += b * shared;
                sum_2 += c * shared;
                sum_3 += d * shared;
            }
            pDstC[i * o + k] = sum;
            pDstC[i * o + (k+1)] = sum_1;
            pDstC[i * o + (k+2)] = sum_2;
            pDstC[i * o + (k+3)] = sum_3;
        }
    }
}

// matrix addition
void mat_add_f32(float * pSrcA, float * pSrcB, uint32_t M, uint32_t N, float * pDst) {
    
    for (int i = 0; i < M * N; i++) {
            pDst[i] = pSrcA[i] + pSrcB[i];
    }
}

// matrix invert function: copied from pulp-dsp to improve it with loop unroll.
int mat_inv_f32(float * pSrc, float * pDst, int N) {

    /*--------------------------------------------------------------------------------------------------------------
     * Matrix Inverse can be solved using elementary row operations.
     *
     *  Gauss-Jordan Method:
     *
     *      1. First combine the identity matrix and the input matrix separated by a bar to form an
     *        augmented matrix as follows:
     *                      _                  _         _         _
     *                     |  a11  a12 | 1   0  |       |  X11 X12  |
     *                     |           |        |   =   |           |
     *                     |_ a21  a22 | 0   1 _|       |_ X21 X21 _|
     *
     *      2. In our implementation, pDst Matrix is used as identity matrix.
     *
     *      3. Begin with the first row. Let i = 1.
     *
     *      4. Check to see if the pivot for row i is zero.
     *         The pivot is the element of the main diagonal that is on the current row.
     *         For instance, if working with row i, then the pivot element is aii.
     *         If the pivot is zero, exchange that row with a row below it that does not
     *         contain a zero in column i. If this is not possible, then an inverse
     *         to that matrix does not exist.
     *
     *      5. Divide every element of row i by the pivot.
     *
     *      6. For every row below and  row i, replace that row with the sum of that row and
     *         a multiple of row i so that each new element in column i below row i is zero.
     *
     *      7. Move to the next row and column and repeat steps 2 through 5 until you have zeros
     *         for every element below and above the main diagonal.
     *
     *      8. Now an identical matrix is formed to the left of the bar(input matrix, pSrc).
     *         Therefore, the matrix to the right of the bar is our solution(pDst matrix, pDst).
     *----------------------------------------------------------------------------------------------------------------*/

    float *pSrcT1, *pSrcT2;                    /* Temporary input data matrix pointer */
    float *pDstT1, *pDstT2;                    /* Temporary output data matrix pointer */
    float *pPivotRowIn;                        /* Temporary input and output data matrix pointer */
    float *pPRT_in, *pPivotRowDst, *pPRT_pDst; /* Temporary input and output data matrix pointer */

    float Xchg, in = 0.0f, in1;                      /* Temporary input values  */
    uint32_t i, rowCnt, flag = 0U, j, loopCnt, k, l; /* loop counters */

    uint32_t M = N; /* M is the number of rows. However, the matirces must be square. */

    /* Working pointer for destination matrix */
    pDstT1 = pDst;

    /* Loop over the number of rows */
    rowCnt = M;

    /* Making the destination matrix as identity matrix */
    while (rowCnt > 0U) {
        /* Writing all zeroes in lower triangle of the destination matrix */
        j = M - rowCnt;
        while (j > 0U) {
            *pDstT1++ = 0.0f;
            j--;
        }

        /* Writing all ones in the diagonal of the destination matrix */
        *pDstT1++ = 1.0f;

        /* Writing all zeroes in upper triangle of the destination matrix */
        j = rowCnt - 1U;
        while (j > 0U) {
            *pDstT1++ = 0.0f;
            j--;
        }

        /* Decrement loop counter */
        rowCnt--;
    }

    /* Loop over the number of columns of the input matrix.
       All the elements in each column are processed by the row operations */
    loopCnt = N;

    /* Index modifier to navigate through the columns */
    l = 0U;

    while (loopCnt > 0U) {
        /* Check if the pivot element is zero..
         * If it is zero then interchange the row with non zero row below.
         * If there is no non zero element to replace in the rows below,
         * then the matrix is Singular. */

        /* Working pointer for the input matrix that points
         * to the pivot element of the particular row  */
        pSrcT1 = pSrc + (l * N);

        /* Working pointer for the destination matrix that points
         * to the pivot element of the particular row  */
        pDstT1 = pDst + (l * N);

        /* Temporary variable to hold the pivot value */
        in = *pSrcT1;

        /* Destination pointer modifier */
        k = 1U;

        /* Check if the pivot element is zero */
        if (*pSrcT1 == 0.0f) {
            /* Loop over the number rows present below */

            for (i = (l + 1U); i < M; i++) {
                /* Update the input and destination pointers */
                pSrcT2 = pSrcT1 + (N * i);
                pDstT2 = pDstT1 + (N * k);

                /* Check if there is a non zero pivot element to
                 * replace in the rows below */
                if (*pSrcT2 != 0.0f) {
                    /* Loop over number of columns
                     * to the right of the pilot element */
                    j = N - l;

                    while (j > 0U) {
                        /* Exchange the row elements of the input matrix */
                        Xchg = *pSrcT2;
                        *pSrcT2++ = *pSrcT1;
                        *pSrcT1++ = Xchg;

                        /* Decrement the loop counter */
                        j--;
                    }

                    /* Loop over number of columns of the destination matrix */
                    j = N;

                    while (j > 0U) {
                        /* Exchange the row elements of the destination matrix */
                        Xchg = *pDstT2;
                        *pDstT2++ = *pDstT1;
                        *pDstT1++ = Xchg;

                        /* Decrement loop counter */
                        j--;
                    }

                    /* Flag to indicate whether exchange is done or not */
                    flag = 1U;

                    /* Break after exchange is done */
                    break;
                }

                /* Update the destination pointer modifier */
                k++;

                /* Decrement loop counter */
            }
        }

        /* Update the status if the matrix is singular */
        if ((flag != 1U) && (in == 0.0f)) {
            return 1;
        }

        /* Points to the pivot row of input and destination matrices */
        pPivotRowIn = pSrc + (l * N);
        pPivotRowDst = pDst + (l * N);

        /* Temporary pointers to the pivot row pointers */
        pSrcT1 = pPivotRowIn;
        pSrcT2 = pPivotRowDst;

        /* Pivot element of the row */
        in = *pPivotRowIn;

        /* Loop over number of columns
         * to the right of the pilot element */
        j = (N - l);

        while (j > 0U) {
            /* Divide each element of the row of the input matrix
             * by the pivot element */
            in1 = *pSrcT1;
            *pSrcT1++ = in1 / in;

            /* Decrement the loop counter */
            j--;
        }

        /* Loop over number of columns of the destination matrix */
        j = N;

        while (j > 0U) {
            /* Divide each element of the row of the destination matrix
             * by the pivot element */
            in1 = *pSrcT2;
            *pSrcT2++ = in1 / in;

            /* Decrement the loop counter */
            j--;
        }

        /* Replace the rows with the sum of that row and a multiple of row i
         * so that each new element in column i above row i is zero.*/

        /* Temporary pointers for input and destination matrices */
        pSrcT1 = pSrc;
        pSrcT2 = pDst;

        /* index used to check for pivot element */
        i = 0U;

        /* Loop over number of rows */
        /*  to be replaced by the sum of that row and a multiple of row i */
        k = M;

        while (k > 0U) {
            /* Check for the pivot element */
            if (i == l) {
                /* If the processing element is the pivot element,
                   only the columns to the right are to be processed */
                pSrcT1 += N - l;

                pSrcT2 += N;
            } else {
                /* Element of the reference row */
                in = *pSrcT1;

                /* Working pointers for input and destination pivot rows */
                pPRT_in = pPivotRowIn;
                pPRT_pDst = pPivotRowDst;

                /* Loop over the number of columns to the right of the pivot element,
                   to replace the elements in the input matrix */
                j = (N - l);

                while (j > 0U) {
                    /* Replace the element by the sum of that row
                       and a multiple of the reference row  */
                    in1 = *pSrcT1;
                    *pSrcT1++ = in1 - (in * *pPRT_in++);

                    /* Decrement the loop counter */
                    j--;
                }

                /* Loop over the number of columns to
                   replace the elements in the destination matrix */
                j = N;

                while (j > 0U) {
                    /* Replace the element by the sum of that row
                       and a multiple of the reference row  */
                    in1 = *pSrcT2;
                    *pSrcT2++ = in1 - (in * *pPRT_pDst++);

                    /* Decrement loop counter */
                    j--;
                }
            }

            /* Increment temporary input pointer */
            pSrcT1 = pSrcT1 + l;

            /* Decrement loop counter */
            k--;

            /* Increment pivot index */
            i++;
        }

        /* Increment the input pointer */
        pSrc++;

        /* Decrement the loop counter */
        loopCnt--;

        /* Increment the index modifier */
        l++;
    }

    if ((flag != 1U) && (in == 0.0f)) {
        for (i = 0; i < M * N; i++) {
            if (pSrc[i] != 0.0f)
                break;
        }

        if (i == M * N)
            return 1;
    }

    return 0;
}

