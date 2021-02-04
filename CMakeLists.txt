cmake_minimum_required(VERSION 3.0)
project(priamcql CXX)

option(PRIAM_BUILD_EXAMPLES                 "Build the examples.  Default=ON" ON)
option(PRIAM_BUILD_TESTS                    "Build the tests.  Default=ON" ON)
option(PRIAM_CODE_COVERAGE                  "Enable code coverage, tests must also be enabled. Default=OFF" OFF)
option(PRIAM_BUILD_EMBEDDED_DATASTAX_DRIVER "Build the bundled datastax cpp driver. Default=ON" ON)

if(NOT DEFINED PRIAM_USER_LINK_LIBRARIES)
    set(
        PRIAM_USER_LINK_LIBRARIES
        uv pthread z dl
        CACHE STRING
        "Override ${PROJECT_NAME} required link libraries, defaults to [uv pthread z dl].  If changed all defaults must be accounted for manually."
    )
endif()

message("${PROJECT_NAME} PRIAM_BUILD_EXAMPLES                 = ${PRIAM_BUILD_EXAMPLES}")
message("${PROJECT_NAME} PRIAM_BUILD_TESTS                    = ${PRIAM_BUILD_TESTS}")
message("${PROJECT_NAME} PRIAM_CODE_COVERAGE                  = ${PRIAM_CODE_COVERAGE}")
message("${PROJECT_NAME} PRIAM_USER_LINK_LIBRARIES            = ${PRIAM_USER_LINK_LIBRARIES}")
message("${PROJECT_NAME} PRIAM_BUILD_EMBEDDED_DATASTAX_DRIVER = ${PRIAM_BUILD_EMBEDDED_DATASTAX_DRIVER}")

if(PRIAM_BUILD_EMBEDDED_DATASTAX_DRIVER)
    set(CASS_BUILD_SHARED   OFF CACHE BOOL "enable libcassandra shared build." FORCE)
    set(CASS_BUILD_STATIC   ON CACHE BOOL "enable libcassandra static build." FORCE)
    set(CASS_USE_STD_ATOMIC ON CACHE BOOL "enable libcassandra std::atomic." FORCE)
    # The libcassandra cmake file doesn't appear to correctly export the include directories.
    set(CASSANDRA_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/datastax/cassandra-cpp-driver/include")
    add_subdirectory(vendor/datastax/cassandra-cpp-driver)
    target_compile_options(cassandra_static PRIVATE
        -Wno-implicit-int-float-conversion
    )
endif()

set(PRIAM_SOURCE_FILES
    inc/priam/blob.hpp
    inc/priam/client.hpp src/client.cpp
    inc/priam/cluster.hpp src/cluster.cpp
    inc/priam/consistency.hpp src/consistency.cpp
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
    inc/priam/status.hpp src/status.cpp
    inc/priam/tuple.hpp src/tuple.cpp
    inc/priam/type.hpp src/type.cpp
    inc/priam/uuid_generator.hpp src/uuid_generator.cpp
    inc/priam/value.hpp src/value.cpp
)

add_library(${PROJECT_NAME} STATIC ${PRIAM_SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# For ubuntu this needs to be linked prior to the system deps
if(PRIAM_BUILD_EMBEDDED_DATASTAX_DRIVER)
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CASSANDRA_INCLUDE})
    target_link_libraries(${PROJECT_NAME} PUBLIC cassandra_static)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${PRIAM_USER_LINK_LIBRARIES})

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

