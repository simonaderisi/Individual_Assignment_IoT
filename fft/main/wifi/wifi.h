/**
 * Set here your wifi credentials anc  the number of times that you want  to retry the connection
*/
#define WIFI_SSID      "SimonaWifi"//TIM-63262762"
#define WIFI_PASS      "password"//"tkx7NXEuN7UDsURceSffU97e"
#define MAXIMUM_RETRY  5

#define WIFI_CONNECTED_BIT BIT0     // we are connected to the  Access Point with an IP
#define WIFI_FAIL_BIT      BIT1     // we failed to connect after the maximum amount of retries

/**
 * @brief Handles WiFi and IP events.
 *
 * @param arg User-defined data, passed directly to the handler.
 * @param event_base The event base identifier.
 * @param event_id The specific event ID.
 * @param event_data Data associated with the event.
 */
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);


/**
 * @brief Initializes the WiFi station mode.
 *
 * This function sets up the WiFi configuration, registers event handlers,
 * and manages the connection process to the Access Point (AP).
 */
void wifi_init_sta(void);
