#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "math_utils.h"

float compute_mean(float *array, int N){
    float sum = 0.0;

    for (int i = 0; i < N; i++) {
        sum += *(array+i);
    }
    float mean = sum / (N);
    return mean;
}


float compute_sd(float *array, int N){
    float sd = 0.0;
    int mean = compute_mean(array, N);

    for (int i = 0; i < N; i++) {
        sd += pow(*(array+i) - mean, 2);
    }
    sd = sqrt(sd / (N));
    return sd;
}


float compute_aggregate_function(float *array, int N){
    float aggregated_value = compute_mean(array, N);
    return aggregated_value;
}


float find_max(float *array, int N){
    float max = 0;
    for(int i=0; i<N; i++){
        if (array[i] > max){
            max = array[i];
        }
    }
    return max;
}