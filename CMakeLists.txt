cmake_minimum_required(VERSION 3.16)

project(cybercon VERSION 0.1 LANGUAGES C)

set(INCLUDE_FILES
)

set(SRC_FILES
    main.c
)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(cybercon ${INCLUDE_FILES} ${SRC_FILES})
target_include_directories(cybercon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)