if(CONFIG_CSI_APP_TYPE_SENDER)
    set(SRCS "send_manager.c")
elseif (CONFIG_CSI_APP_TYPE_RECEIVER)
    set(SRCS "recv_manager.c")
else()
    set(SRCS "")
endif()

idf_component_register(SRCS "main.c"
                            "init_manager.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash esp_event esp_wifi)