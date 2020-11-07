cmake_minimum_required(VERSION 2.8)
project(priamcql CXX)

option(PRIAM_BUILD_EXAMPLES "Build the examples.  Default=ON" ON)
option(PRIAM_BUILD_TESTS    "Build the tests.  Default=ON" ON)
option(PRIAM_CODE_COVERAGE  "Enable code coverage, tests must also be enabled. Default=OFF" OFF)

message("${PROJECT_NAME} PRIAM_BUILD_EXAMPLES = ${PRIAM_BUILD_EXAMPLES}")
message("${PROJECT_NAME} PRIAM_BUILD_TESTS    = ${PRIAM_BUILD_TESTS}")
message("${PROJECT_NAME} PRIAM_CODE_COVERAGE  = ${PRIAM_CODE_COVERAGE}")

# Set some general system dependencies
set(PRIAM_SYSTEM_LIBRARY_DEPENDENCIES
    uv
    pthread
    z
    dl
)

set(CASS_BUILD_SHARED OFF CACHE BOOL "enable libcassandra shared build." FORCE)
set(CASS_BUILD_STATIC ON CACHE BOOL "enable libcassandra static build." FORCE)
set(CASS_USE_STD_ATOMIC ON CACHE BOOL "enable libcassandra std::atomic." FORCE)
# The libcassandra cmake file doesn't appear to correctly export the include directories.
set(CASSANDRA_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/datastax/cassandra-cpp-driver/include")
add_subdirectory(vendor/datastax/cassandra-cpp-driver)
target_compile_options(cassandra_static PRIVATE
    -Wno-implicit-int-float-conversion
)

set(PRIAM_SOURCE_FILES
    inc/priam/blob.hpp
    inc/priam/client.hpp src/client.cpp
    inc/priam/cluster.hpp src/cluster.cpp
    inc/priam/cpp_driver.hpp
    inc/priam/decimal.hpp
    inc/priam/duration.hpp
    inc/priam/list.hpp src/list.cpp
    inc/priam/map.hpp src/map.cpp
    inc/priam/prepared.hpp src/prepared.cpp
    inc/priam/priam.hpp
    inc/priam/result.hpp src/result.cpp
    inc/priam/row.hpp src/row.cpp
    inc/priam/set.hpp src/set.cpp
    inc/priam/statement.hpp src/statement.cpp
    inc/priam/tuple.hpp src/tuple.cpp
    inc/priam/type.hpp
    inc/priam/uuid_generator.hpp src/uuid_generator.cpp
    inc/priam/Value.hpp src/Value.cpp
)

add_library(${PROJECT_NAME} STATIC ${PRIAM_SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CASSANDRA_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE cassandra_static)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES})

if(PRIAM_CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wno-unknown-pragmas
        -Wall
        -Wextra
        -Weffc++
        -Werror
        -Wpedantic
        -pedantic-errors
    )
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Weffc++
        -Werror
        -Wpedantic
        -pedantic-errors
    )
endif()

if(PRIAM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PRIAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

