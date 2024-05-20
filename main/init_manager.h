#ifndef ESP_CSI_INIT_MANAGER_H
#define ESP_CSI_INIT_MANAGER_H

/**
 * @brief Runs the initialization manager.
 *
 * This function is responsible for running the initialization manager. It is typically called at the start of the application.
 * The initialization manager is responsible for setting up the necessary resources and state for the application to run.
 *
 * @param arg A pointer to the argument data for the initialization manager. The exact usage of this argument depends on the implementation of the initialization manager.
 */
void run_init_task(void *arg);

ESP_EVENT_DECLARE_BASE(INIT_MANAGER_EVENTS);

typedef enum {
    WIFI_INIT_SUCCESS_EVENT,
    WIFI_INIT_FAIL_EVENT,
    CSI_RECV_INIT_SUCCESS_EVENT,
    CSI_RECV_INIT_FAIL_EVENT,
    NVS_INIT_SUCCESS_EVENT,
    NVS_INIT_FAIL_EVENT,
    INIT_MANAGER_ANY_EVENT,
} init_manager_events_t;

/**
 * @brief Runs the WiFi initialization task.
 *
 * This function is responsible for running the WiFi initialization task. It is typically called by the initialization manager.
 * The WiFi initialization task is responsible for setting up the necessary resources and state for the WiFi functionality of the application.
 * @attention 3. The bit 0 of the first byte of MAC address can not be 1. For example, the MAC address
 *      can set to be "1a:XX:XX:XX:XX:XX", but can not be "15:XX:XX:XX:XX:XX".
 * @param arg A pointer to the argument data for the WiFi initialization task. The exact usage of this argument depends on the implementation of the WiFi initialization task.
 */
void wifi_init_task(void *arg);
// TODO: document this function
void csi_recv_init_task(void *arg);
// TODO: document this function
void nvs_init_task(void *arg);
// TODO: document this function
void esp_now_init_task(void *arg);

#endif //ESP_CSI_INIT_MANAGER_H
