cmake_minimum_required(VERSION 3.16)
project(stm32_ecu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(stm32_ecu
    App/src/vehicle_state.c
    App/src/fault_manager.c
)

target_include_directories(stm32_ecu
    PUBLIC
        App/include
        config
)

# Example placeholder; real STM32 toolchain configuration should be added later.
