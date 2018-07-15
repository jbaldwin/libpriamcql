cmake_minimum_required(VERSION 3.0.2)
project(priamcql CXX)

# Create a Debug/lib or Release/lib output directory.
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib)

# Custom libcassandra includes for compiling.
if(NOT "$ENV{CASSANDRA_INCLUDE}" STREQUAL "")
    set(CASSANDRA_INCLUDE $ENV{CASSANDRA_INCLUDE})
else()
    set(CASSANDRA_INCLUDE "" CACHE STRING "User specified libcassandra include location.")
endif()

# Custom libcassandra.so|a file location for linking.
if(NOT "$ENV{LIBCASSANDRA}" STREQUAL "")
    set(LIBCASSANDRA $ENV{LIBCASSANDRA})
else()
    set(LIBCASSANDRA "libcassandra.so" CACHE STRING "User specified libcassandra location.")
endif()

option(D_GLIBCXX_DEBUG "Use D_GLIBCXX_DEBUG when building Debug" ON)

message("${PROJECT_NAME} CASSANDRA_INCLUDE = ${CASSANDRA_INCLUDE}")
message("${PROJECT_NAME} LIBCASSANDRA = ${LIBCASSANDRA}")
message("${PROJECT_NAME} D_GLIBCXX_DEBUG=${D_GLIBCXX_DEBUG}")

# Set some general system dependencies
set(PRIAM_SYSTEM_LIBRARY_DEPENDENCIES
    uv
    pthread
    z
    dl
)

set(SOURCE_FILES
    inc/priam/Client.h src/Client.cpp
    inc/priam/Cluster.h src/Cluster.cpp
    inc/priam/Value.h src/Value.cpp
    inc/priam/CppDriver.h
    inc/priam/CQL.h
    inc/priam/List.h inc/priam/List.tcc src/List.cpp
    inc/priam/Map.h inc/priam/Map.tcc src/Map.cpp
    inc/priam/Prepared.h src/Prepared.cpp
    inc/priam/Result.h inc/priam/Result.tcc src/Result.cpp
    inc/priam/Row.h inc/priam/Row.tcc src/Row.cpp
    inc/priam/Set.h inc/priam/Set.tcc src/Set.cpp
    inc/priam/Tuple.h inc/priam/Tuple.tcc src/Tuple.cpp
    inc/priam/Statement.h src/Statement.cpp
)

##### Export the Library target #####

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
if(NOT "${CASSANDRA_INCLUDE}" STREQUAL "")
    target_include_directories(${PROJECT_NAME} PUBLIC ${CASSANDRA_INCLUDE})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBCASSANDRA}
    ${PRIAM_SYSTEM_LIBRARY_DEPENDENCIES}
)

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

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("${PROJECT_NAME} is building Debug")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -O0
            -g
            -gdwarf-2
    )
    if(D_GLIBCXX_DEBUG)
        message("${PROJECT_NAME} D_GLIBCXX_DEBUG is ON")
        target_compile_options(${PROJECT_NAME} PRIVATE -D_GLIBCXX_DEBUG)
    else()
        message("${PROJECT_NAME} D_GLIBCXX_DEBUG is OFF")
    endif()

else() # "Release"
    message("${PROJECT_NAME} is building Release")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -O2
            -g
            -gdwarf-2
    )
endif()

include(examples/CMakeLists.txt)
