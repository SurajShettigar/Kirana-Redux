message("Adding Library: app")
add_library(app src/application.cpp)
target_include_directories(app PUBLIC include)
target_link_libraries(app PUBLIC core renderer)