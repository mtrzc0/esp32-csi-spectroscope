idf_component_register(SRCS "main.c"
                            "init_manager.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash esp_event esp_wifi)
