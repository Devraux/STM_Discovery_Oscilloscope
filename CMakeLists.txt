cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
set(CMAKE_PROJECT_NAME STM_Discovery_Oscilloscope)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})

message("Build type: " ${CMAKE_BUILD_TYPE})

enable_language(ASM)

add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(cmake/stm32cubemx)

file(GLOB_RECURSE USER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/Core/Src/*.c
    ${CMAKE_SOURCE_DIR}/display_lib/*.c
    ${CMAKE_SOURCE_DIR}/middlewares/ui/*.c
    ${CMAKE_SOURCE_DIR}/middlewares/lvgl/src/*.c
)

list(REMOVE_ITEM USER_SOURCES
    ${CMAKE_SOURCE_DIR}/Core/Src/system_stm32f7xx.c
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${USER_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE

    ${CMAKE_SOURCE_DIR}/Core/Inc

    ${CMAKE_SOURCE_DIR}/Core/Src/STM32F746_Hardware_Lib

    ${CMAKE_SOURCE_DIR}/display_lib

    ${CMAKE_SOURCE_DIR}/middlewares
    ${CMAKE_SOURCE_DIR}/middlewares/lvgl
    ${CMAKE_SOURCE_DIR}/middlewares/lvgl/src
    ${CMAKE_SOURCE_DIR}/middlewares/ui

    ${CMAKE_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy

    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F7xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include

    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Include
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE

    STM32F746xx
    USE_HAL_DRIVER

    LV_CONF_INCLUDE_SIMPLE

    ARM_MATH_CM7
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Lib/GCC/libarm_cortexM7lfsp_math.a

)


list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)