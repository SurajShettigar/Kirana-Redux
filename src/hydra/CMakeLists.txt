message("Adding Library: HdKirana")
add_library(HdKirana SHARED src/mesh.cpp src/render_pass.cpp src/render_delegate.cpp src/renderer_plugin.cpp)
target_include_directories(HdKirana PUBLIC include)
target_link_libraries(HdKirana PUBLIC usdImaging PRIVATE core renderer)
target_compile_definitions(HdKirana PUBLIC NOMINMAX PRIVATE $<$<CONFIG:Debug>:TBB_USE_DEBUG>)
set_target_properties(HdKirana PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(HdKiranaTest test/main.cpp)
target_link_libraries(HdKiranaTest PRIVATE HdKirana)
set_target_properties(HdKiranaTest PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)