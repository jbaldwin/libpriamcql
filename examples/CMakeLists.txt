cmake_minimum_required(VERSION 2.8)
project(libpriamcql_examples CXX)

### async ###
add_executable(priam_async async.cpp)
target_link_libraries(priam_async PRIVATE priamcql)

### bench ###
add_executable(priam_bench bench.cpp)
target_link_libraries(priam_bench PRIVATE priamcql)

### readme ###
add_executable(priam_readme readme.cpp)
target_link_libraries(priam_readme PRIVATE priamcql)
