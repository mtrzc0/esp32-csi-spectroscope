# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 3.5)
add_compile_options(-fdiagnostics-color=always)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(esp-csi-sensor-fusion)

#show all environment variables
execute_process(
        COMMAND ${CMAKE_COMMAND} -E environment
)
#message("EXTRA_COMPONENT_DIRS: " ${EXTRA_COMPONENT_DIRS})

#string(REGEX REPLACE ".*/\(.*\)" "\\1" CURDIR temp/csi_recv)

#git_describe(PROJECT_VERSION ${COMPONENT_DIR})
#message("Project commit: " ${PROJECT_VERSION})
