include_directories(${PROJECT_SOURCE_DIR}/Src)

add_executable(table_scan_benchmark table_scan_benchmark.cpp)

target_link_libraries(table_scan_benchmark benchmark lib_static pthread)