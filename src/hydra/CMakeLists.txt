message("Adding Library: HdKirana")
add_library(HdKirana SHARED
        src/mesh.cpp
        src/render_pass.cpp
        src/render_delegate.cpp
        src/renderer_plugin.cpp)
target_include_directories(HdKirana PUBLIC include)
target_compile_definitions(HdKirana PUBLIC NOMINMAX PRIVATE $<$<CONFIG:Debug>:TBB_USE_DEBUG>)
target_link_libraries(HdKirana PUBLIC usd usdImaging PRIVATE core renderer)

#set(RESOURCE_DIR ${CMAKE_BINARY_DIR}/lib/usd/hdKirana/)
#if (NOT EXISTS RESOURCE_DIR)
#    file(MAKE_DIRECTORY ${RESOURCE_DIR})
#endif ()
#add_custom_command(TARGET HdKirana PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${RESOURCE_DIR}
#        COMMAND_EXPAND_LISTS)
add_executable(HdKiranaTest test/main.cpp)
target_link_libraries(HdKiranaTest PRIVATE HdKirana)