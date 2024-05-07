cmake_minimum_required(VERSION 3.16)

project(cybercon VERSION 0.1 LANGUAGES C)

find_package(SDL2 REQUIRED)

set(INCLUDE_FILES
)

set(SRC_FILES
    main.c
)

set(VENDOR_FILES
    include/KHR/khrplatform.h
    include/glad/gl.h
    src/gl.c
)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(TRANSFORM VENDOR_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/vendor/)

add_executable(cybercon ${INCLUDE_FILES} ${SRC_FILES} ${VENDOR_FILES})

target_include_directories(cybercon
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
        ${SDL2_INCLUDE_DIRS}
)

target_link_directories(cybercon
    PUBLIC
        ${SDL2_LIBRARIES}
)

target_link_libraries(cybercon
    mingw32
    SDL2main
    SDL2
)