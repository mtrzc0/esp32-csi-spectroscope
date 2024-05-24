#ifndef ESP_CSI_INIT_MANAGER_H
#define ESP_CSI_INIT_MANAGER_H

/**
 * @brief Runs the WiFi initialization task.
 *
 * This function is responsible for running the WiFi initialization task. It is typically called by the initialization manager.
 * The WiFi initialization task is responsible for setting up the necessary resources and state for the WiFi functionality of the application.
 * @attention 3. The bit 0 of the first byte of MAC address can not be 1. For example, the MAC address
 *      can set to be "1a:XX:XX:XX:XX:XX", but can not be "15:XX:XX:XX:XX:XX".
 */
void wifi_init();
// TODO: document this function
void csi_recv_init();
// TODO: document this function
void nvs_init();

#endif //ESP_CSI_INIT_MANAGER_H
