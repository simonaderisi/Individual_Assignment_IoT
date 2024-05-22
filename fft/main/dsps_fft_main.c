#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include "wifi/wifi.h"
#include "mqtt/mqtt.h"
#include "utils/math_utils.h"
#include "fft/fft.h"
#include "sensing/sensing.h"


int N = NO_OF_SAMPLES;

int sample_rate = SAMPLE_RATE;

int TIME_WINDOW = 5;    //seconds on which I want to calculate the aggregate function

char *TAG_MAIN = "MAIN";


/**
 * Initialize all the needed channel of communication. WiFi, MQTT and ADC.
*/
void  initialization(){

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //initialize WiFi
    wifi_init_sta();
    vTaskDelay(10000/ portTICK_PERIOD_MS);
    ESP_LOGI(TAG_MAIN, "WiFi initialized");
    //initialize MQTT
    mqtt_app_start();
    ESP_LOGI(TAG_MAIN, "MQTT initialized");
    //initialize ADC
    initializeADC();
    ESP_LOGI(TAG_MAIN, "ADC initialized");
}


void fft(){
    //get the signal
    float* my_signal = sensing(sample_rate);

    //my_signal = remove_DC();

    //compute fft on the signal
    float *y = calculate_fft(&my_signal[0]);

    //get dominant frequency
    int dominantFrequency = get_dominant_frequency(&y[0]);

    //calculate nyquist frequency
    int nyquist = (dominantFrequency * 2) + 1;

    //update sample rate
    if (nyquist < SAMPLE_RATE){
        sample_rate = nyquist;
        vTaskDelay(pdMS_TO_TICKS(INTERVAL));
    }
}


void aggregated_function(){
    //compute number of measurements that we need to do to have data on a time window of TIME_WINDOW
    int number_of_measurements = ceil((float)TIME_WINDOW * (float)sample_rate / (float)NO_OF_SAMPLES);
    ESP_LOGI(TAG_MAIN, "With sample rate %d Hz, we need %d measurements.", sample_rate, number_of_measurements);
    float aggregate_intermediates [number_of_measurements];     //to store intermediate aggregated valued
    float* my_signal;
    for (int i=0; i<number_of_measurements; i++){
        my_signal = sensing(sample_rate);
        aggregate_intermediates[i] = compute_aggregate_function(&my_signal[0], N);
    }
    float aggregated_value = compute_aggregate_function(&aggregate_intermediates[0], number_of_measurements);
    ESP_LOGW(TAG_MAIN, "aggregated value is %f\n", aggregated_value);

    //communicate aggregated value through MQTT
    char value[10];
    sprintf(value, "%f", aggregated_value);

    // Calcolare la lunghezza necessaria per il messaggio finale
    const char *prefix = "Aggregated value is ";
    size_t message_length = strlen(prefix) + strlen(value) + 1; // +1 per il terminatore nullo

    // allocate memory for final message
    char *message = (char *)malloc(message_length * sizeof(char));
    if (message == NULL) {
        // Gestione dell'errore di allocazione della memoria
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Costruire il messaggio finale
    strcpy(message, prefix);
    strcat(message, value);

    mqtt_publish(message);
}



void app_main()
{
    ESP_LOGI(TAG_MAIN, "INIT");

    initialization();

    mqtt_publish("Starting calculation with maximum sample rate");

    aggregated_function();

    fft();

    // while(1){
    //     mqtt_publish("Starting calculation with maximum sample rate");

    //     aggregated_function();
    // }

    

    

    mqtt_publish("Starting calculation with adjusted sample rate");
    
    aggregated_function();
}
