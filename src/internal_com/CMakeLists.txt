cmake_minimum_required(VERSION 3.10)
project(internal_com)

list(APPEND INTERNAL_COM_SRC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/internal_com.cpp
)

add_library(INTERNAL_COM STATIC ${INTERNAL_COM_SRC})

target_include_directories(INTERNAL_COM PUBLIC 
  ${CMAKE_SOURCE_DIR}/include/internal_com
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(INTERNAL_COM 
  rt
  pthread
)

set_target_properties(INTERNAL_COM PROPERTIES PUBLIC_HEADER
                      "${CMAKE_SOURCE_DIR}/include/internal_comm/internal_com.h"
                     )
