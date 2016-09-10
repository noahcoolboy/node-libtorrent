cmake_minimum_required(VERSION 3.1)

# Name of the project (will be the name of the plugin)
project (node-libtorrent-ng)

# specify sources
file(GLOB MODULE_SOURCES "src/*.cpp")

# we're building a shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCES})

# set .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Include BoostLib module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
message(${CMAKE_MODULE_PATH})
include(BoostLib)

set(LIBTORRENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/deps/libtorrent)
set(LIBTORRENT_HEADERS ${LIBTORRENT_SOURCES}/include)

# locate/download boost
require_boost_libs(">= 1.57.0" thread;date_time;filesystem)

# fortunately libtorrent has its own cmake
add_subdirectory(${LIBTORRENT_SOURCES})

# specify includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBTORRENT_HEADERS})

# specify libs
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
#target_link_libraries(${PROJECT_NAME} ${LIBTORRENT_SOURCES})