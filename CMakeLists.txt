# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 3.5)
add_compile_options(-fdiagnostics-color=always)

set(CMAKE_FIND_ROOT_PATH $ENV{HOME}/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/)
set(CMAKE_CXX_COMPILER  $ENV{HOME}/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-g++)
set(CMAKE_C_COMPILER  $ENV{HOME}/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc)
set(CMAKE_ASM_COMPILER  $ENV{HOME}/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp-csi-sensors)

#show all environment variables
#execute_process(
#        COMMAND ${CMAKE_COMMAND} -E environment
#)