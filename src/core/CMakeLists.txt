message("Adding Library: core")
add_library(core
        src/logger.cpp
        src/type_conversions.cpp
        src/image.cpp
        src/input_manager.cpp
        src/time_manager.cpp
        src/job_manager.cpp
        src/window.cpp
        src/window_manager.cpp)
target_include_directories(core PUBLIC include PRIVATE internal)
target_link_libraries(core PRIVATE spdlog::spdlog cppcodec OpenImageIO::OpenImageIO)