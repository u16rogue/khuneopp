cmake_minimum_required(VERSION 3.18)

if (NOT TARGET khuneo)
  message(FATAL_ERROR "-- khuneopp > Target 'khuneo' was not found or isn't available. Please ensure that the target is available before adding khuneopp.")
endif()

project(khuneopp)

add_library(
  ${PROJECT_NAME}
  STATIC
  "include/khpp/khuneopp.hpp"
  "src/khuneopp.cpp"

  "include/khpp-core/refobj.hpp"
  "include/khpp-core/types.hpp"

  "include/khpp-astgen/lexer.hpp"
  "src/astgen/lexer.cpp"

  "include/khpp-astgen/parser.hpp"
  "src/astgen/parser.cpp"

)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  "include/"
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  CXX_STANDARD 20
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  khuneo
)
