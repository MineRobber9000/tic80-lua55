cmake_minimum_required(VERSION 3.12)
project(tic80-core-example VERSION 0.1.0 LANGUAGES C CXX)

# I copied these from TIC-80 itself just in case it matters
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_C_FLAGS_RELEASE "-O2 -flto -ffat-lto-objects ${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -ffat-lto-objects ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"  OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
set(CMAKE_C_FLAGS_RELEASE "-O2 -flto ${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
endif()

set(TIC80_DIR "${CMAKE_SOURCE_DIR}/../TIC-80")

add_library(examplecore SHARED ${CMAKE_SOURCE_DIR}/src/main.c)
target_include_directories(examplecore PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${TIC80_DIR}/include
	${TIC80_DIR}/src
)
set_target_properties(examplecore PROPERTIES
	PREFIX "")
# If you want textri/OVR stuff:
# target_compile_definitions(examplecore PRIVATE BUILD_DEPRECATED)
