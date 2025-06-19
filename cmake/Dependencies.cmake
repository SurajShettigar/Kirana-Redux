# Catch2 (Testing Framework)
# Link: https://github.com/catchorg/Catch2
add_subdirectory(ext/Catch2)

# CPPCodec (Base64 encoding-decoding)
# Link: https://github.com/tplgy/cppcodec
add_library(cppcodec INTERFACE)
target_include_directories(cppcodec INTERFACE ext/cppcodec)

# spdlog (Logging)
# Link: https://github.com/gabime/spdlog
add_subdirectory(ext/spdlog)

# Vulkan - Graphics API
# Link: https://www.lunarg.com/vulkan-sdk/
find_package(Vulkan REQUIRED)
if (${Vulkan_FOUND})
    message("Found Vulkan: " ${Vulkan_VERSION})
endif ()

# Python
# Link: https://www.python.org/
find_package(Python REQUIRED)

# OpenUSD
# Link: https://github.com/PixarAnimationStudios/OpenUSD
set(OpenUSD_ROOT_DIR ${PROJECT_SOURCE_DIR}/ext/OpenUSD)
set(OpenUSD_BUILD_SCRIPT_FILE ${OpenUSD_ROOT_DIR}/build_scripts/build_usd.py)
set(OpenUSD_BUILD_OUTPUT_DIR ${OpenUSD_ROOT_DIR}/build)
set(OpenUSD_INCLUDE_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/include)
set(OpenUSD_LIB_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/lib)
set(OpenUSD_BIN_DIR ${OpenUSD_BUILD_OUTPUT_DIR}/bin)

if (NOT EXISTS ${OpenUSD_INCLUDE_DIR} OR NOT EXISTS ${OpenUSD_LIB_DIR})
    execute_process(
            COMMAND ${Python_EXECUTABLE}
            ${OpenUSD_BUILD_SCRIPT_FILE}
            ${OpenUSD_BUILD_OUTPUT_DIR}
            --no-python
            --no-examples
            --no-tutorials
            --no-tools
            --no-imaging
            --no-materialx)
endif ()
include(${OpenUSD_BUILD_OUTPUT_DIR}/pxrConfig.cmake)
