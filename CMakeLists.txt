cmake_minimum_required(VERSION 2.8)
project(priamcql CXX)

# Create a Debug/lib or Release/lib output directory.
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib)

# Custom libcassandra.so|a file location for linking.
if(NOT "$ENV{LIBCASSANDRA}" STREQUAL "")
    set(LIBCASSANDA $ENV{LIBCASSANDRA})
else()
    set(LIBCASSANDRA "libcassandra.so" CACHE STRING "User specified libcassandra location.")
endif()
message("libcassandra = ${LIBCASSANDRA}")

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
    inc/priam/Prepared.h src/Prepared.cpp
    inc/priam/Result.h inc/priam/Result.tcc src/Result.cpp
    inc/priam/Row.h inc/priam/Row.tcc src/Row.cpp
    inc/priam/Statement.h src/Statement.cpp
)

##### Export the Library target #####

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

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
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -O0
            -g
            -gdwarf-2
            -D_GLIBCXX_DEBUG
    )
else() # "Release"
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
            -O2
            -g
            -gdwarf-2
    )
endif()

include(examples/CMakeLists.txt)
