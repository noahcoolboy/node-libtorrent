cmake_minimum_required(VERSION 3.1)

# debug purposes
set(CMAKE_VERBOSE_MAKEFILE ON)

# Name of the project (will be the name of the plugin)
project (node-libtorrent-ng)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(LIBTORRENT_LIB "torrent-rasterbar")

if(UNIX)
    add_definitions(-fPIC)
    add_definitions(-fvisibility=hidden)

    if(APPLE)
        # few tweaks for osx
        add_definitions(-mmacosx-version-min=10.7)
        add_definitions(-stdlib=libc++)
        add_definitions(-D__STDC_CONSTANT_MACROS)
        set(CMAKE_MACOSX_RPATH ON)
    else(APPLE)
        # supress this warning on linux
        add_definitions(-Wno-potentially-evaluated-expression)
    endif(APPLE)
else(UNIX)
    # assuming we're on windows here
    add_definitions(-DBOOST_ASIO_DYN_LINK -DBOOST_DATE_TIME_DYN_LINK -DBOOST_THREAD_DYN_LINK -DBOOST_FILESYSTEM_DYN_LINK)
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # multicore compilation
        add_definitions(/EHsc)
    endif()
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(UNIX)

# Include BoostLib module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
include(BoostLib)

# locate/download boost
require_boost_libs(">= 1.57.0" thread;date_time;filesystem)

# set up libtorrent
set(LIBTORRENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/deps/libtorrent)
set(LIBTORRENT_HEADERS ${LIBTORRENT_SOURCES}/include)

# set libtorrent build params
set(shared OFF CACHE BOOL "" FORCE)
set(geoip OFF CACHE BOOL "" FORCE)
set(encryption OFF CACHE BOOL "" FORCE)

# fortunately it has its own cmake
add_subdirectory(${LIBTORRENT_SOURCES})

# specify module sources
file(GLOB MODULE_SOURCES "src/*.cpp")

# we're building a shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCES} ${LIBTORRENT_SOURCES})

# 1. set .node extension without any "lib" prefix
# 2. allow it to find libtorrent.so in the same dir
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node") # BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH ".")

# specify includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBTORRENT_HEADERS})

# specify libs
target_link_libraries(${PROJECT_NAME} ${LIBTORRENT_LIB})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
