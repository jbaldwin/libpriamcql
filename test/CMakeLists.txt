cmake_minimum_required(VERSION 2.8)
project(libpriamcql_tests CXX)

SET(LIBPRIAMCQL_TEST_SOURCE_FILES
    test_async.cpp
    test_keyspace.cpp
    test_types.cpp
    test_uuid_generator.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${LIBPRIAMCQL_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE priamcql)

if(PRIAM_CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

add_test(NAME PriamCQLTest COMMAND ${PROJECT_NAME})
