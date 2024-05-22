#include "mqtt_client.h"

#define MQTT_BROKER "mqtts://test.mosquitto.org:8883"
#define MQTT_TOPIC_OUTPUT "project/fft/output"
#define MQTT_TOPIC_INPUT "project/fft/input"

/**
 * @brief Callback function to handle MQTT events.
 *
 * @param event The event data.
 * @return esp_err_t The result of the event handling.
 */
esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event);


/**
 * @brief Event handler function for MQTT events.
 *
 * @param handler_args Arguments passed to the handler.
 * @param base The event base identifier.
 * @param event_id The specific event ID.
 * @param event_data Data associated with the event.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);


/**
 * @brief Publishes a message to a predefined MQTT topic.
 *
 * @param message The message to be published.
 */
void mqtt_publish(char *message);


/**
 * @brief Initializes and starts the MQTT client.
 *
 * @return esp_mqtt_client_handle_t, the handle to the initialized MQTT client
 */
esp_mqtt_client_handle_t mqtt_app_start(void);
