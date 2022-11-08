cmake_minimum_required(VERSION 3.10)
project(SOCKET_PUBSUB)

list(APPEND SOCKET_PUBSUB_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_publisher.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_subscriber.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_subscriber_service.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_publisher_service.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_pubsub_server.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/socket_filter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/socket_pubsub.cpp
)

add_library(SOCKET_PUBSUB STATIC ${SOCKET_PUBSUB_SRC})

target_include_directories(SOCKET_PUBSUB PUBLIC 
  ${CMAKE_SOURCE_DIR}/include/socket_pubsub
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIR}
)

target_link_libraries(SOCKET_PUBSUB 
  rt
  pthread
#   -L${Boost_LIBRARY_DIR}
#     boost_thread
#     boost_system
#     boost_filesystem
#     boost_regex
#     boost_date_time
#     boost_chrono
#     boost_atomic
#     pthread

)

set_target_properties(SOCKET_PUBSUB PROPERTIES PUBLIC_HEADER
                      "${CMAKE_SOURCE_DIR}/include/socket_pubsub/socket_pubsub.h;${CMAKE_SOURCE_DIR}/include/socket_pubsub/observer.h;${CMAKE_SOURCE_DIR}/include/socket_pubsub/socket_def_type.h"
                     )
