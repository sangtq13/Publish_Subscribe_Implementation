cmake_minimum_required(VERSION 3.10)
project(IPC_PUBSUB)

list(APPEND IPC_PUBSUB_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/ipc_pubsub.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ipc_publisher.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ipc_subscriber.cpp
)

add_library(IPC_PUBSUB STATIC ${IPC_PUBSUB_SRC})

target_include_directories(IPC_PUBSUB PUBLIC 
  ${CMAKE_SOURCE_DIR}/include/ipc_pubsub
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(IPC_PUBSUB 
  rt
  pthread
)

set_target_properties(IPC_PUBSUB PROPERTIES PUBLIC_HEADER
                      "${CMAKE_SOURCE_DIR}/include/ipc_pubsub/ipc_pubsub.h;${CMAKE_SOURCE_DIR}/include/ipc_pubsub/observer.h"
                     )
