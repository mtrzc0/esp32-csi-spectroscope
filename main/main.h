#ifndef MAIN_H
#define MAIN_H

ESP_EVENT_DECLARE_BASE(APP_MAIN_EVENTS);

/**
 * @brief Enumeration for the main application events.
 *
 * This enumeration defines the possible events that can be posted to the APP_MAIN_EVENTS event base.
 * The events are:
 * - INIT_MANAGER_RUN_EVENT: This event is posted when the initialization manager starts running.
 * - INIT_MANAGER_SUCCESS_EVENT: This event is posted when the initialization manager successfully completes its task.
 * - INIT_MANAGER_FAIL_EVENT: This event is posted when the initialization manager fails to complete its task.
 * - INIT_MANAGER_ANY_EVENT: This event is a catch-all for any event posted to the APP_MAIN_EVENTS event base.
 */
typedef enum {
    INIT_MANAGER_RUN_EVENT,
    INIT_MANAGER_SUCCESS_EVENT,
    INIT_MANAGER_FAIL_EVENT,
    INIT_MANAGER_ANY_EVENT,
}app_main_events_t;

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
static void init_manager_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif //MAIN_H
