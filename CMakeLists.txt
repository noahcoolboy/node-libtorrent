cmake_minimum_required(VERSION 3.1)

# Name of the project (will be the name of the plugin)
project (node-libtorrent-ng)

# Include BoostLib module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
message(${CMAKE_MODULE_PATH})
include(BoostLib)

set(LIBTORRENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/deps/libtorrent)
set(LIBTORRENT_HEADERS ${LIBTORRENT_SOURCE}/include)

# locate/download boost
require_boost_libs(">= 1.57.0" thread;date_time;filesystem)

# fortunately libtorrent has its own cmake
add_subdirectory(${LIBTORRENT_SOURCES})

# specify sources
file(GLOB MODULE_SOURCES "src/*.cpp")

# we're building a shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCES})

# set .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# specify includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBTORRENT_HEADERS})
#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
# Define Boost dependencies there.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})