add_executable(dense_flow_1 dense_flow_1.cpp)
target_link_libraries(dense_flow_1 PRIVATE cp_lib)

add_executable(bfs bfs.cpp)
target_link_libraries(bfs PRIVATE cp_lib)

add_executable(binary_lifting binary_lifting.cpp)
target_link_libraries(binary_lifting PRIVATE cp_lib)

add_executable(mst_n_binary_lifting mst_n_binary_lifting.cpp)
target_link_libraries(mst_n_binary_lifting PRIVATE cp_lib)

add_executable(twoSAT twoSAT.cpp)
target_link_libraries(twoSAT PRIVATE cp_lib)

add_executable(dense_flow2_BS dense_flow2_BS.cpp)
target_link_libraries(dense_flow2_BS PRIVATE cp_lib)
