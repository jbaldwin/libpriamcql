cmake_minimum_required(VERSION 2.8)
project(vidarcql CXX)

message(${CMAKE_CXX_COMPILER_ID})

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
ELSEIF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-weak-vtables -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-exit-time-destructors -Wno-global-constructors -Wno-error=deprecated-declarations -Wno-disabled-macro-expansion -Wno-covered-switch-default -fcolor-diagnostics -ferror-limit=128")
ELSE()
    message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER}")
ENDIF()

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Weffc++ -Werror -Wpedantic -pedantic-errors -MD ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -gdwarf-2 -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g -gdwarf-2")

message("CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
    inc/vidar/Client.h src/Client.cpp
    inc/vidar/Cluster.h src/Cluster.cpp
    inc/vidar/Column.h src/Column.cpp
    inc/vidar/ConnectionInfo.h src/ConnectionInfo.cpp
    inc/vidar/CQL.h
    inc/vidar/OnCompleteCallback.h
    inc/vidar/Prepared.h src/Prepared.cpp
    inc/vidar/Result.h src/Result.cpp
    inc/vidar/ResultIterator.h src/ResultIterator.cpp
    inc/vidar/Row.h src/Row.cpp
    inc/vidar/Statement.h src/Statement.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE
    uv
    pthread
    z
    dl
)
