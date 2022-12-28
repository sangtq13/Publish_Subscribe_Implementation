#
# (c) 2021 Copyright, VINAI
#

cmake_minimum_required (VERSION 3.10.0)
project (PubSub)
# set(CMAKE_VERBOSE_MAKEFILE on)

if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif()

set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_EXTENSIONS OFF)
set (DTC_LIBRARIES ${CMAKE_PROJECT_NAME})

############################## Build flag ##############################
if (CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -lrt -Werror -Wno-reorder -fPIC")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -lrt -Werror -Wno-reorder -fPIC")
else()
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -lrt -Wall -Wextra -Wno-reorder -fPIC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -lrt -Wall -Wextra -Wno-reorder -fPIC")
endif()

############################## Set CMake module ##############################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/socket_pubsub)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ipc_pubsub)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/internal_com)
