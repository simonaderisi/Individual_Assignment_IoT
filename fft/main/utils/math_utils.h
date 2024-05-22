/**
 * @brief Computes the mean value of the array.
 * 
 * @param array Pointer to the array.
 * @param N Length of the array.
 * @return The mean value.
 */
float compute_mean(float *array, int N);


/**
 * @brief Computes the standard deviation of the array.
 * 
 * @param array Pointer to the array.
 * @param N Length of the array.
 * @return The standard deviation.
 */
float compute_sd(float *array, int N);


/**
 * @brief Computes the aggregation function chosen for the signal.
 * 
 * In this case, it computes the mean value.
 * 
 * @param array Pointer to the array containing the signal.
 * @param N Length of the array.
 * @return The aggregated value.
 */
float compute_aggregate_function(float *array, int N);


/**
 * @brief Returns the maximum value in the array.
 * 
 * @param array Pointer to the array.
 * @param N Length of the array.
 * @return The maximum value.
 */
float find_max(float *array, int N);
