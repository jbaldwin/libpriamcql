cmake_minimum_required(VERSION 2.8)
project(libpriamcql_tests CXX)

SET(LIBPRIAMCQL_TEST_SOURCE_FILES

)

add_executable(${PROJECT_NAME} main.cpp ${LIBPRIAMCQL_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE priamcql)
add_test(NAME PriamCQLTest COMMAND ${PROJECT_NAME})
