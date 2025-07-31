add_library(scene internal/gltf.cpp src/scene.cpp src/loader.cpp)
target_include_directories(scene PUBLIC include PRIVATE internal)
target_link_libraries(scene PUBLIC core glaze::glaze)

add_executable(sceneTest test/main.cpp)
target_link_libraries(sceneTest PRIVATE scene)