#include "pmsis.h"

int push(float val, int free_space, float * buff) {

    /* if the buffer is to small, then double its size
    if ((*free_space) == 0) {
        // update head of the buffer
        (*free_space) += (*current_len);

        // update current total length of the buffer
        int new_length = (*current_len) * 2;
        (*current_len) = new_length;

        // allocate a new buffer
        float * new_arr = (float *) malloc(new_length * sizeof(float));
        
        // init new buffer with the previous array
        for(int i = (new_length - 1); i > new_length/2; i--) {
            new_arr[i] = arr[i - new_length/2];
        }

        for(int i = 0; i <= new_length/2; i++) {
            new_arr[i] = 0.0f;
        }

        // change to new array
        //free(arr);
        arr = new_arr;
        
    }*/

    // push element at head
    free_space--;
    buff[free_space] = val;
    return free_space;
}


int flush(int n, float * buff, int free_space, int buff_len) {
    
    // remove n element from the head of the buffer
    if (n > (buff_len - free_space))
        n = (buff_len - free_space);

    free_space += n;

    // shift elements in array on the right 
    for(int i = (buff_len - n - 1); i > 0; i--) 
        buff[i + n] = buff[i];

    return free_space;
}
int size();
float get_item(int n);