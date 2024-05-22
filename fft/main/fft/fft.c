#include "freertos/FreeRTOS.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "esp_dsp.h"
#include <math.h>

#include "fft.h"
#include "../utils/math_utils.h"

char *TAG_FFT = "FFT";

// window coefficients
__attribute__((aligned(16)))
float window[NO_OF_SAMPLES];
__attribute__((aligned(16)))
float y_cf[NO_OF_SAMPLES * 2];

float y[NO_OF_SAMPLES/2];       //store output magnitudes

float* calculate_fft(float *my_signal){
    esp_err_t ret;
    ESP_LOGI(TAG_FFT, "Start FFT");
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK) {
        ESP_LOGE(TAG_FFT, "Not possible to initialize FFT. Error = %i", ret);
        exit(1);
    }

    // Generate hann window
    dsps_wind_hann_f32(window, NO_OF_SAMPLES);


    // Convert two input vectors to one complex vector
    for (int i = 0 ; i < NO_OF_SAMPLES ; i++) {
        y_cf[i * 2 + 0] = *(my_signal +i) * window[i];
        y_cf[i * 2 + 1] = 0.0; //imaginary part is null
    }

    // FFT
    dsps_fft2r_fc32(y_cf, NO_OF_SAMPLES);
    // Bit reverse
    dsps_bit_rev_fc32(y_cf, NO_OF_SAMPLES);
    // Convert one complex vector to two complex vectors
    dsps_cplx2reC_fc32(y_cf, NO_OF_SAMPLES);


    for (int i = 0 ; i < NO_OF_SAMPLES / 2 ; i++) {
        y[i] = 10 * log10f((y_cf[i*2] * y_cf[i*2] + y_cf[i*2 + 1] * y_cf[i*2 + 1]) / NO_OF_SAMPLES);
    }

    ESP_LOGI(TAG_FFT, "FFT finished");
    return &y[0];
}


int get_dominant_frequency(float *y){
    int bin = 0;
    float mean = compute_mean(y, NO_OF_SAMPLES/2);
    float sd = compute_sd(y, NO_OF_SAMPLES/2);
    float z;
    for (int i=0; i< NO_OF_SAMPLES/2; i++){
        z = (*(y+i) - mean) / sd;       //z-score
        if (z > 3.1){     //consider only outliers
            bin = i;
        }
    }

    int dominantFrequency = bin * SAMPLE_RATE / NO_OF_SAMPLES;

    ESP_LOGI(TAG_FFT, "Dominant Frequency: %d Hz", dominantFrequency);
    return dominantFrequency;
}