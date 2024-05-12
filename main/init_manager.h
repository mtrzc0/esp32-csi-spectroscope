#ifndef ESP_CSI_SENSOR_FUSION_INIT_MANAGER_H
#define ESP_CSI_SENSOR_FUSION_INIT_MANAGER_H

/**
 * @brief Runs the initialization manager.
 *
 * This function is responsible for running the initialization manager. It is typically called at the start of the application.
 * The initialization manager is responsible for setting up the necessary resources and state for the application to run.
 *
 * @param arg A pointer to the argument data for the initialization manager. The exact usage of this argument depends on the implementation of the initialization manager.
 */
void run_init_manager(void *arg);

ESP_EVENT_DECLARE_BASE(INIT_MANAGER_EVENTS);

/**
 * @brief Event handler for the initialization manager.
 *
 * This function is an event handler for the initialization manager. It is called when an event is posted to the APP_MAIN_EVENTS event base.
 * It handles two types of events: INIT_MANAGER_SUCCESS_EVENT and INIT_MANAGER_FAIL_EVENT. For any other event, it logs a warning message.
 *
 * @param arg A pointer to the argument data for the event. This is not used in this function.
 * @param event_base The base of the event. This function only handles events with the base APP_MAIN_EVENTS.
 * @param event_id The ID of the event. This function handles events with the IDs INIT_MANAGER_SUCCESS_EVENT and INIT_MANAGER_FAIL_EVENT.
 * @param event_data A pointer to the event data. This is not used in this function.
 */
typedef enum {
    WIFI_INIT_SUCCESS_EVENT,
    WIFI_INIT_FAIL_EVENT,
    CSI_INIT_SUCCESS_EVENT,
    CSI_INIT_FAIL_EVENT,
    NVS_INIT_SUCCESS_EVENT,
    NVS_INIT_FAIL_EVENT,
    ESP_NOW_INIT_SUCCESS_EVENT,
    ESP_NOW_INIT_FAIL_EVENT,
    INIT_MANAGER_ANY_EVENT,
};

void wifi_init(void *arg);

#endif //ESP_CSI_SENSOR_FUSION_INIT_MANAGER_H
