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
void run_init(void *arg);

ESP_EVENT_DECLARE_BASE(INIT_MANAGER_EVENTS);

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

// TODO: document this function
void wifi_init_task(void *arg);
// TODO: document this function
void csi_init_task(void *arg);

#endif //ESP_CSI_SENSOR_FUSION_INIT_MANAGER_H
