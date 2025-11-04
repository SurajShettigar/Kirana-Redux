cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE} CACHE PATH "Application/library install directory" FORCE)
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