cmake_minimum_required(VERSION 2.8)
project(libpriamcql_examples CXX)

### simple ###
set(SIMPLE_SOURCE_FILES
    simple.cpp
)
add_executable(priam_simple ${SIMPLE_SOURCE_FILES})
target_link_libraries(priam_simple PRIVATE priamcql)

### bench ###
set(SIMPLE_SOURCE_FILES
    bench.cpp
)
add_executable(priam_bench ${SIMPLE_SOURCE_FILES})
target_link_libraries(priam_bench PRIVATE priamcql)
