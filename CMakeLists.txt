cmake_minimum_required(VERSION 2.8)
project(priamcql CXX)

option(PRIAM_BUILD_EXAMPLES "Build the examples.  Default=ON" ON)
option(PRIAM_BUILD_TESTS    "Build the tests.  Default=ON" ON)

message("${PROJECT_NAME} PRIAM_BUILD_EXAMPLES = ${PRIAM_BUILD_EXAMPLES}")
message("${PROJECT_NAME} PRIAM_BUILD_TESTS    = ${PRIAM_BUILD_TESTS}")

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
    inc/priam/Blob.hpp
    inc/priam/Client.hpp src/Client.cpp
    inc/priam/Cluster.hpp src/Cluster.cpp
    inc/priam/CppDriver.hpp
    inc/priam/CQL.hpp
    inc/priam/Decimal.hpp
    inc/priam/Duration.hpp
    inc/priam/List.hpp inc/priam/List.tcc src/List.cpp
    inc/priam/Map.hpp inc/priam/Map.tcc src/Map.cpp
    inc/priam/Prepared.hpp src/Prepared.cpp
    inc/priam/Result.hpp inc/priam/Result.tcc src/Result.cpp
    inc/priam/Row.hpp inc/priam/Row.tcc src/Row.cpp
    inc/priam/Set.hpp inc/priam/Set.tcc src/Set.cpp
    inc/priam/Statement.hpp src/Statement.cpp
    inc/priam/Tuple.hpp inc/priam/Tuple.tcc src/Tuple.cpp
    inc/priam/UuidGenerator.hpp src/UuidGenerator.cpp
    inc/priam/Value.hpp src/Value.cpp
)

##### Export the Library target #####

add_library(${PROJECT_NAME} STATIC ${PRIAM_SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CASSANDRA_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE cassandra_static)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES})

if(PRIAM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PRIAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

