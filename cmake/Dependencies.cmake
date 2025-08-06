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

# CPPCodec (Base64 encoding-decoding)
# Link: https://github.com/tplgy/cppcodec
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec")
if (CPPCODEC_INCLUDE_DIRS STREQUAL "")
    message(FATAL_ERROR "Library not found: cppcodec")
endif ()
add_library(cppcodec INTERFACE)
target_include_directories(cppcodec INTERFACE ${CPPCODEC_INCLUDE_DIRS})

# Glaze (JSON)
# Link: https://stephenberry.github.io/glaze/
find_package(glaze CONFIG REQUIRED)

# spdlog (Logging)
# Link: https://github.com/gabime/spdlog
find_package(spdlog CONFIG REQUIRED)

## OpenImageIO
## Link: https://github.com/AcademySoftwareFoundation/OpenImageIO
find_package(OpenImageIO CONFIG REQUIRED)

# OpenUSD
# Link: https://github.com/PixarAnimationStudios/OpenUSD
find_package(pxr CONFIG REQUIRED)