#define DEFAULT_VREF    1100        // Reference value for the ADC
#define NO_OF_SAMPLES   1024        // Number of  samples for the FFT
#define SAMPLE_RATE     1000        // Sample rate in Hz
#define INTERVAL        1000        // Interval of time to update the sample rate
#define ADC_UNIT ADC_UNIT_1         //ADC unit of the  ESP32
#define ADC_CHANNEL ADC1_CHANNEL_1  //ADC channel


/**
 * @brief Removes the DC component from the signal.
 * 
 * Useful to remove the component at frequence 0 that may interfere in the FFT results
 * 
 * @return A pointer to the processed signal array.
 */
float* remove_DC(void);

/**
 * @brief Normalizes the signal.
 *
 * @return A pointer to the normalized signal array.
 */
float* normalize(void);

/**
 * @brief Initializes the ADC configuration.
 * 
 * Configures the ADC width, channel attenuation, and characterizes the ADC for the specified unit.
 */
void initializeADC(void);

/**
 * @brief Samples the signal at a specified SAMPLE_RATE.
 * 
 * @param sample_rate The sampling frequency in Hz.
 * 
 * @return A pointer to the sampled signal array.
 */
float* sensing(int sample_rate);