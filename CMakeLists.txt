cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

#set(BUILD_SHARED_LIBS "Build using shared libraries" ON)
project(Kirana)
add_executable(Kirana src/main.cpp)

# External dependencies
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Platform)
include(Dependencies)
include(Shaders)

# Internal dependencies
add_subdirectory(src/core)
add_subdirectory(src/scene)
add_subdirectory(src/renderer)
add_subdirectory(src/hydra)
add_subdirectory(src/app)

target_link_libraries(Kirana PUBLIC app)