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
    APP_MAIN_ANY_EVENT,
}app_main_events_t;

#endif //MAIN_H
