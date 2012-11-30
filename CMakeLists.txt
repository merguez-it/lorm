cmake_minimum_required(VERSION 2.8)

# Issue ??? -- TO BE REMOVED
if(APPLE)
  set(CMAKE_CXX_COMPILER g++)
  set(CMAKE_C_COMPILER gcc)
endif(APPLE)

project(LORM)

set(CMAKE_MODULE_PATH ${LORM_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(FindDoxygen)
include(FindSqlite3)
include(FindPostgreSQL)
include(FindMySql)

if(SQLITE3_FOUND)
  set(HAVE_SQLITE 1)
  include_directories(${SQLITE3_INCLUDE_DIR})
endif()
if(MYSQL_FOUND)
  set(HAVE_MYSQL 1)
  include_directories(${MYSQL_INCLUDE_DIR})
endif()
if(PostgreSQL_FOUND) 
  set(HAVE_POSTGRESQL 1)
  include_directories(${PostgreSQL_INCLUDE_DIRS})
endif()

set(LORM_OUTPUT_TEMP_DIR ${CMAKE_BINARY_DIR}/.build)
file(MAKE_DIRECTORY ${LORM_OUTPUT_TEMP_DIR})

add_definitions(-Wall)
if(DEBUG)
  set(CMAKE_C_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_CXX_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

if(CPP11) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${LORM_OUTPUT_TEMP_DIR}/config.h
  )

# Documentation
if(DOXYGEN_FOUND)
  find_program(GRAPHVIZ
    dot
    )
  if("${GRAPHVIZ}" STREQUAL "GRAPHVIZ-NOTFOUND")
    message(STATUS "GraphViz not found!")
    set(HAVE_DOT NO)
  else()
    message(STATUS "Found GraphViz: ${GRAPHVIZ}")
    set(HAVE_DOT YES)
  endif()
  configure_file(
    ${LORM_SOURCE_DIR}/Doxyfile.in 
    ${LORM_OUTPUT_TEMP_DIR}/Doxyfile
    )

  set(DOXYGEN_INPUT ${LORM_OUTPUT_TEMP_DIR}/Doxyfile)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT})
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(ext)
add_subdirectory(test)
add_subdirectory(examples)
