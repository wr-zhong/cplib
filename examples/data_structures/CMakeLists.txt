add_executable(dsu dsu.cpp)
target_link_libraries(dsu PRIVATE cp_lib)

add_executable(range_tree range_tree.cpp)
target_link_libraries(range_tree PRIVATE cp_lib)

add_executable(lazy_range_tree lazy_range_tree.cpp)
target_link_libraries(lazy_range_tree PRIVATE cp_lib)
