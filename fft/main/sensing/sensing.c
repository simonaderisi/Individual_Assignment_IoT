#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "dsps_tone_gen.h"
#include "esp_timer.h"
#include <math.h>

#include "sensing.h"
#include "../utils/math_utils.h"


static float my_signal[NO_OF_SAMPLES];     //will contain the sampled signal

static esp_adc_cal_characteristics_t adc_chars;     // to deal with the adc of the esp32 to be able to sample

char *TAG_SENSING = "SENSING";


float* remove_DC(){
    float mean = compute_mean(&my_signal[0], NO_OF_SAMPLES);
    for(int i=0; i<NO_OF_SAMPLES; i++){
        my_signal[i] -= mean;
    }
    return &my_signal[0];
}


float* normalize(){
    float max  = find_max(&my_signal[0], NO_OF_SAMPLES);
    for (int  i=0; i<NO_OF_SAMPLES; i++){
        my_signal[i] /= max;
    }
    return &my_signal[0];
}


void initializeADC(){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_0);
    esp_adc_cal_characterize(ADC_UNIT, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_12));
    ESP_LOGI(TAG_SENSING, "Configured ADC");
}


float* sensing(int sample_rate){
    ESP_LOGI(TAG_SENSING, "Sampling at %d Hz", sample_rate);
    float t;
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        my_signal[i] = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC_CHANNEL), &adc_chars);
        //t = (float)i / sample_rate;
        //my_signal[i] = 2 * sin(2 * M_PI * 40 * t) + 4 * sin(2 * M_PI * 25 * t);
        vTaskDelay(pdMS_TO_TICKS(1000 / sample_rate));      //delay between two samples 
    }
    return &my_signal[0];
}