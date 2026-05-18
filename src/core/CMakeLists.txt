message("Adding Library: core")

set(CORE_SOURCES
        src/logger.cpp
        src/type_conversions.cpp
        src/event_manager.cpp
        src/input_manager.cpp
        src/time_manager.cpp
        src/job_manager.cpp
        src/window_manager.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
    list(APPEND CORE_SOURCES
            platform/windows/win32_window.cpp
            platform/windows/win32_input.cpp)
elseif (${CMAKE_SYSTEM_NAME} MATCHES Linux)
    list(APPEND CORE_SOURCES
            platform/linux/x11_window.cpp
            platform/linux/x11_input.cpp)
elseif (${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    list(APPEND CORE_SOURCES
            platform/darwin/cocoa_window.cpp
            platform/darwin/cocoa_input.cpp)
endif ()

add_library(core ${CORE_SOURCES})
target_compile_definitions(core PRIVATE PLATFORM=${CURRENT_PLATFORM})
target_include_directories(core PUBLIC include PRIVATE platform/windows platform/linux platform/darwin)
target_link_libraries(core PRIVATE spdlog::spdlog cppcodec)

if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
    target_compile_definitions(core PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif ()