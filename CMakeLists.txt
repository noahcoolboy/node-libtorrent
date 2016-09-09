cmake_minimum_required(VERSION 3.1)

# Name of the project (will be the name of the plugin)
project (node-libtorrent-ng)

# Include BoostLib module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
message(${CMAKE_MODULE_PATH})
include(BoostLib)

set(LIBTORRENT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/deps/libtorrent)
set(LIBTORRENT_PREFIX ${CMAKE_BINARY_DIR}/libtorrent)

# locate/download boost
require_boost_libs(">= 1.57.0" thread;date_time;filesystem)
require_boost_libs()

add_subdirectory(${LIBTORRENT_SOURCE})

# specify sources
set(MODULE_SOURCE module 
				  add_torrent_params.cpp
				  alert
        		  bencode
        		  create_torrent
        		  entry
                  file_storage
                  fingerprint
                  ip_filter
                  peer_info
                  rss
                  session
                  session_settings
                  session_status
                  storage
                  torrent_handle
                  torrent_info
                  torrent_status)

# specify includes
include_directories("deps/libtorrent/include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_JS_INC})

# we're building a shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCE})

# set .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
# Define Boost dependencies there.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB};${Boost_LIBRARIES})