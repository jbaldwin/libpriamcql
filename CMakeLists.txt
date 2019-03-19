cmake_minimum_required(VERSION 3.0.2)
project(priamcql CXX)

# Create a Debug/lib or Release/lib output directory.
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib)

set(CASSANDRA_INCLUDE "" CACHE PATH "User specified libcassandra include location.")
set(LIBCASSANDRA "libcassandra.so" CACHE STRING "User specified libcassandra location.")

message("${PROJECT_NAME} CASSANDRA_INCLUDE = ${CASSANDRA_INCLUDE}")
message("${PROJECT_NAME} LIBCASSANDRA = ${LIBCASSANDRA}")

# Set some general system dependencies
set(PRIAM_SYSTEM_LIBRARY_DEPENDENCIES
    uv
    pthread
    z
    dl
)

set(SOURCE_FILES
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

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CASSANDRA_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBCASSANDRA}
    ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES}
)

if(UNIX)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Weffc++
            -Werror
            -Wpedantic
            -pedantic-errors
            -MD
    )
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -Wno-unknown-pragmas
    )
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-padded
            -Wno-weak-vtables
            -Wno-gnu-anonymous-struct
            -Wno-nested-anon-types
            -Wno-exit-time-destructors
            -Wno-global-constructors
            -Wno-error=deprecated-declarations
            -Wno-disabled-macro-expansion
            -Wno-covered-switch-default
            -fcolor-diagnostics
            -ferror-limit=128
    )
endif()

include(examples/CMakeLists.txt)
