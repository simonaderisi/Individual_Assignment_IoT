#include "../sensing/sensing.h"

/**
 * @brief Calculates the FFT of the signal stored in my_signal.
 * 
 * This function initializes the FFT, applies a Hann window to the signal,
 * computes the FFT, and converts the result to a power spectrum in dB.
 * 
 * @param my_signal Pointer to an array where the signal is stored.
 * 
 * @return Pointer to an array where the power spectrum will be stored.
*/
float* calculate_fft(float *my_signal);

/**
 * @brief Finds the dominant frequency in the power spectrum.
 * 
 * This function calculates the mean and standard deviation of the power spectrum,
 * and identifies the frequency with the highest z-score greater than 3.
 * 
 * @param y Pointer to the power spectrum array.
 * 
 * @return The dominant frequency in the power spectrum.
*/
int get_dominant_frequency(float *y);