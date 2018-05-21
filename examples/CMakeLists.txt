cmake_minimum_required(VERSION 2.8)
project(libvidarcql_examples)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin)

### simple ###
set(SIMPLE_SOURCE_FILES
    examples/simple.cpp
)
add_executable(simple ${SIMPLE_SOURCE_FILES})
target_link_libraries(simple PRIVATE vidarcql ${LIBCASSANDRA} ${SYSTEM_LIBRARY_DEPENDENCIES})
