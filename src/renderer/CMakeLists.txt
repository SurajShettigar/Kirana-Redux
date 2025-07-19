message("Adding Library: renderer")
add_library(renderer
        src/device.cpp
        src/memory_allocator.cpp
        src/synchronization.cpp
        src/texture.cpp
        src/swapchain.cpp
        src/descriptor_layout.cpp
        src/descriptor_set.cpp
        src/descriptor_allocator.cpp
        src/pipeline_layout.cpp
        src/shader.cpp
        src/pipeline_compute.cpp
        src/pipeline_graphics.cpp
        src/command_encoder.cpp
        src/queue.cpp
        src/renderer.cpp)
target_include_directories(renderer
        PUBLIC include)
target_link_libraries(renderer PUBLIC core Vulkan::Vulkan)

# Vulkan header pre-processor directives
target_compile_definitions(renderer PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)
target_compile_definitions(renderer PRIVATE PLATFORM=${CURRENT_PLATFORM})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(renderer PUBLIC DEBUG=1)
endif ()