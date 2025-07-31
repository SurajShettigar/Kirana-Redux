# CPPCodec (Base64 encoding-decoding)
# Link: https://github.com/tplgy/cppcodec
add_library(cppcodec INTERFACE)
target_include_directories(cppcodec INTERFACE ext/cppcodec)

# spdlog (Logging)
# Link: https://github.com/gabime/spdlog
add_subdirectory(ext/spdlog)

# Glaze (JSON)
#Link: https://stephenberry.github.io/glaze/
add_subdirectory(ext/glaze)

# Vulkan - Graphics API
# Link: https://www.lunarg.com/vulkan-sdk/
find_package(Vulkan REQUIRED)
if (${Vulkan_FOUND})
    message("Adding Library: Vulkan " ${Vulkan_VERSION})
    cmake_path(GET Vulkan_GLSLC_EXECUTABLE PARENT_PATH Vulkan_BINARY)
    if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
        set(SLANG_EXE_NAME "slangc.exe")
    else ()
        set(SLANG_EXE_NAME "slangc")
    endif ()
    cmake_path(APPEND Vulkan_BINARY ${SLANG_EXE_NAME} OUTPUT_VARIABLE Vulkan_SLANG_EXECUTABLE)
    if (EXISTS ${Vulkan_SLANG_EXECUTABLE})
        message("Found Slang Shader Compiler: " ${Vulkan_SLANG_EXECUTABLE})
    else ()
        message(SEND_ERROR "Cannot find slang shader compiler. Make sure the Vulkan SDK has slang compiler.")
    endif ()
endif ()

# Python
# Link: https://www.python.org/
find_package(Python REQUIRED)

# OpenUSD
# Link: https://github.com/PixarAnimationStudios/OpenUSD
set(OpenUSD_ROOT_DIR ${PROJECT_SOURCE_DIR}/ext/OpenUSD)
set(OpenUSD_BUILD_SCRIPT_FILE ${OpenUSD_ROOT_DIR}/build_scripts/build_usd.py)
set(OpenUSD_BUILD_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(OpenUSD_INCLUDE_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/include)
set(OpenUSD_LIB_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/lib)
set(OpenUSD_BIN_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/bin)
set(OpenUSD_CONFIG_FILE ${OpenUSD_BUILD_OUTPUT_DIR}/pxrConfig.cmake)

if (NOT EXISTS ${OpenUSD_CONFIG_FILE})
# TODO: Build USDView by creating a python virtual environment and executing OpenUSD build script.
    message("Building OpenUSD...")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        execute_process(
                COMMAND ${Python_EXECUTABLE}
                ${OpenUSD_BUILD_SCRIPT_FILE}
                ${OpenUSD_BUILD_OUTPUT_DIR}
                --build-variant=debug
                --no-python
                --no-examples
                --no-tutorials
                --no-tools)
    else ()
        execute_process(
                COMMAND ${Python_EXECUTABLE}
                ${OpenUSD_BUILD_SCRIPT_FILE}
                ${OpenUSD_BUILD_OUTPUT_DIR}
                --no-python
                --no-examples
                --no-tutorials
                --no-tools)
    endif ()
else ()
    message("Skipping OpenUSD build")
endif ()
include(${OpenUSD_CONFIG_FILE})
