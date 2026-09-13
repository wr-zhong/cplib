add_executable(linear_rec linear_rec.cpp)
target_link_libraries(linear_rec PRIVATE cp_lib)

add_executable(spf_sieve spf_sieve.cpp)
target_link_libraries(spf_sieve PRIVATE cp_lib)