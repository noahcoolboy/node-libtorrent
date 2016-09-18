cmake_minimum_required(VERSION 3.1)

# Name of the project (will be the name of the plugin)
project (node-libtorrent-ng)

set(LIBTORRENT_LIBS "torrent-rasterbar")

# Include BoostLib module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
include(BoostLib)

# locate/download boost
require_boost_libs(">= 1.57.0" thread;date_time;filesystem)

# set up libtorrent
set(LIBTORRENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/deps/libtorrent)
set(LIBTORRENT_HEADERS ${LIBTORRENT_SOURCES}/include)

# fortunately it has its own cmake
add_subdirectory(${LIBTORRENT_SOURCES})

# specify module sources
file(GLOB MODULE_SOURCES "src/*.cpp")

# we're building a shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCES} ${LIBTORRENT_SOURCES})

# 1. set .node extension without any "lib" prefix
# 2. allow it to find libtorrent.so in the same dir
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node" SKIP_BUILD_RPATH TRUE)

# specify includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBTORRENT_HEADERS})

# specify libs
target_link_libraries(${PROJECT_NAME} ${LIBTORRENT_LIBS})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
