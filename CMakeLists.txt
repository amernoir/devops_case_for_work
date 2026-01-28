cmake_minimum_required(VERSION 3.10)
project(sqlite3_amalgamation LANGUAGES C)

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling detected")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message(STATUS "Cross-compiling for Windows")
        set(PLATFORM_WINDOWS TRUE)
    endif()
else()
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
            message(FATAL_ERROR "MSVC compiler required for Windows native build")
        endif()
        message(STATUS "Native Windows build with MSVC")
        set(PLATFORM_WINDOWS TRUE)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
            message(WARNING "Expected GCC on Linux, found: ${CMAKE_C_COMPILER_ID}")
        endif()
        message(STATUS "Native Linux build with ${CMAKE_C_COMPILER_ID}")
        set(PLATFORM_LINUX TRUE)
    else()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR WIN32 OR MINGW)
    message(STATUS "Building for Windows platform")
    set(PLATFORM_WINDOWS TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Building for Linux platform")
    set(PLATFORM_LINUX TRUE)
endif()

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

add_library(sqlite3 SHARED src/sqlite3.c)
set_target_properties(sqlite3 PROPERTIES OUTPUT_NAME "sqlite3")

if(PLATFORM_WINDOWS)
    message(STATUS "Configuring for Windows DLL")
    
    # Для Windows DLL
    target_compile_definitions(sqlite3 PRIVATE 
		#    SQLITE_API=__declspec(dllexport)
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_DEPRECATE
    )
    
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")  # MinGW
        message(STATUS "Using MinGW compiler for Windows")
        target_compile_options(sqlite3 PRIVATE
            -Wall
            -Wextra
            -static-libgcc
            -Os
        )
        target_link_options(sqlite3 PRIVATE -static)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Using MSVC compiler")
        target_compile_options(sqlite3 PRIVATE
            /W3
            /wd4996
            /Os
        )
    endif()
    
elseif(PLATFORM_LINUX)
    message(STATUS "Configuring for Linux shared library")
    
    # Для Linux .so
    target_compile_definitions(sqlite3 PRIVATE
        _GNU_SOURCE
        _FILE_OFFSET_BITS=64
    )
    
    target_compile_options(sqlite3 PRIVATE
		# -Wall
		#    -Wextra
		-Wpedantic
        -fPIC
        -fvisibility=hidden
        -Os
    )
    
    set_target_properties(sqlite3 PROPERTIES
        VERSION "3.43.2"
        SOVERSION "3"
    )
endif()
target_include_directories(sqlite3 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_THREADSAFE=1
    SQLITE_ENABLE_EXPLAIN_COMMENTS
    SQLITE_ENABLE_DBSTAT_VTAB
    SQLITE_ENABLE_FTS3
    SQLITE_ENABLE_FTS3_PARENTHESIS
    SQLITE_ENABLE_FTS4
    SQLITE_ENABLE_FTS5
    SQLITE_ENABLE_JSON1
)

message(STATUS "Building sqlite3 as shared library")
message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Cross-compiling: ${CMAKE_CROSSCOMPILING}")
