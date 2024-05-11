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

#endif //ESP_CSI_SENSOR_FUSION_INIT_MANAGER_H
