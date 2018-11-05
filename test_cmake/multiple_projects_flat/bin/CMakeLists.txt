project(add-something)
include_directories(${add_SOURCE_DIR})
add_executable(add-something main.cc)
target_link_libraries(add-something add)
