message("Adding Library: renderer")
add_library(renderer src/device.cpp src/renderer.cpp)
target_include_directories(renderer
        PUBLIC include)
target_link_libraries(renderer PUBLIC core Vulkan::Vulkan)

# Vulkan header pre-processor directives
target_compile_definitions(renderer PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)
target_compile_definitions(renderer PRIVATE PLATFORM=${CURRENT_PLATFORM})