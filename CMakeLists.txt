cmake_minimum_required(VERSION 2.8)
project(priamcql CXX)

# Set some general system dependencies
set(PRIAM_SYSTEM_LIBRARY_DEPENDENCIES
    uv
    pthread
    z
    dl
)

set(CASS_BUILD_STATIC ON CACHE BOOL "enable libcassandra static build." FORCE)
add_subdirectory(vendor/datastax/cassandra-cpp-driver)
# The libcassandra cmake file doesn't appear to correctly export the include directories.
set(CASSANDRA_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/datastax/cassandra-cpp-driver/include")

set(PRIAM_SOURCE_FILES
    inc/priam/Blob.h
    inc/priam/Client.h src/Client.cpp
    inc/priam/Cluster.h src/Cluster.cpp
    inc/priam/CppDriver.h
    inc/priam/CQL.h
    inc/priam/Decimal.h
    inc/priam/Duration.h
    inc/priam/List.h inc/priam/List.tcc src/List.cpp
    inc/priam/Map.h inc/priam/Map.tcc src/Map.cpp
    inc/priam/Prepared.h src/Prepared.cpp
    inc/priam/Result.h inc/priam/Result.tcc src/Result.cpp
    inc/priam/Row.h inc/priam/Row.tcc src/Row.cpp
    inc/priam/Set.h inc/priam/Set.tcc src/Set.cpp
    inc/priam/Statement.h src/Statement.cpp
    inc/priam/Tuple.h inc/priam/Tuple.tcc src/Tuple.cpp
    inc/priam/UuidGenerator.h src/UuidGenerator.cpp
    inc/priam/Value.h src/Value.cpp
)

##### Export the Library target #####

add_library(${PROJECT_NAME} STATIC ${PRIAM_SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CASSANDRA_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE cassandra_static)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES})

add_subdirectory(examples)
enable_testing()
add_subdirectory(test)


