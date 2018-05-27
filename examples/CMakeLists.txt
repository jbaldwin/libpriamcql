cmake_minimum_required(VERSION 2.8)
project(libpriamcql_examples CXX)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin)

### simple ###
set(SIMPLE_SOURCE_FILES
    examples/simple.cpp
)
add_executable(simple ${SIMPLE_SOURCE_FILES})
target_link_libraries(simple PRIVATE priamcql ${LIBCASSANDRA} ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES})

### bench ###
set(SIMPLE_SOURCE_FILES
    examples/bench.cpp
)
add_executable(bench ${SIMPLE_SOURCE_FILES})
target_link_libraries(bench PRIVATE priamcql ${LIBCASSANDRA} ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES})
