cmake_minimum_required(VERSION 3.12)
project(tic80-core-lua55 VERSION 0.1.0 LANGUAGES C CXX)

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
set(LUA55_DIR "${CMAKE_SOURCE_DIR}/src/lua-5.5.1/src")

add_library(lua55 SHARED
	${LUA55_DIR}/lapi.c
	${LUA55_DIR}/lauxlib.c
	${LUA55_DIR}/lbaselib.c
	${LUA55_DIR}/lcode.c
	${LUA55_DIR}/lcorolib.c
	${LUA55_DIR}/lctype.c
	${LUA55_DIR}/ldblib.c
	${LUA55_DIR}/ldebug.c
	${LUA55_DIR}/ldo.c
	${LUA55_DIR}/ldump.c
	${LUA55_DIR}/lfunc.c
	${LUA55_DIR}/lgc.c
	${LUA55_DIR}/linit.c
	${LUA55_DIR}/liolib.c
	${LUA55_DIR}/llex.c
	${LUA55_DIR}/lmathlib.c
	${LUA55_DIR}/lmem.c
	${LUA55_DIR}/loadlib.c
	${LUA55_DIR}/lobject.c
	${LUA55_DIR}/lopcodes.c
	${LUA55_DIR}/loslib.c
	${LUA55_DIR}/lparser.c
	${LUA55_DIR}/lstate.c
	${LUA55_DIR}/lstring.c
	${LUA55_DIR}/lstrlib.c
	${LUA55_DIR}/ltable.c
	${LUA55_DIR}/ltablib.c
	${LUA55_DIR}/ltm.c
	${LUA55_DIR}/lundump.c
	${LUA55_DIR}/lutf8lib.c
	${LUA55_DIR}/lvm.c
	${LUA55_DIR}/lzio.c
	${TIC80_DIR}/src/api/parse_note.c
	${TIC80_DIR}/src/api/luaapi.c
	${CMAKE_SOURCE_DIR}/src/main.c)
target_include_directories(lua55 PRIVATE
	${CMAKE_SOURCE_DIR}/src
        ${LUA55_DIR}
	${TIC80_DIR}/include
	${TIC80_DIR}/src
	${TIC80_DIR}/src/api
)
set_target_properties(lua55 PROPERTIES
	PREFIX "")
# If you want textri/OVR stuff:
target_compile_definitions(lua55 PRIVATE BUILD_DEPRECATED)
