cmake_minimum_required(VERSION 3.18)

if (NOT TARGET khuneo)
  message(FATAL_ERROR "-- khuneopp > Target 'khuneo' was not found or isn't available. Please ensure that the target is available before adding khuneopp.")
endif()

if (NOT TARGET kh_core)
  message(FATAL_ERROR "-- khuneopp > Target 'kh_core' was not found or is inaccessible. khuneo's core component is required.")
endif()

project(khuneopp)

add_library(
  ${PROJECT_NAME}
  STATIC

  "include/khpp/khuneopp.hpp"
  "src/khuneopp.cpp"

  "include/khpp-core/refobj.hpp"
  "include/khpp-core/types.hpp"
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

# ASTGen Component
if (TARGET kh_astgen)
  message("-- khuneopp > ASTGen available!")
  target_sources(
    khuneopp
    PRIVATE 
    "include/khpp-astgen/lexer.hpp"
    "src/astgen/lexer.cpp"
    "include/khpp-astgen/parser.hpp"
    "src/astgen/parser.cpp"
  )
  target_compile_definitions(khuneopp PUBLIC KHPP_ASTGEN=1)

  # Check if extra component for ASTGen is available.
  if (TARGET kh_extra)
    message("-- khuneopp > ASTGen Extra available!")
    target_sources(
      khuneopp
      PRIVATE 
      "include/khpp-extra/astgen/lexer.hpp"
      "src/extra/astgen/lexer.cpp"
    )
    target_compile_definitions(khuneopp PUBLIC KHPP_EXTRA_ASTGEN=1)
  else()
    message("-- khuneopp > ASTGen Extra unavailable!")
    target_compile_definitions(khuneopp PUBLIC KHPP_EXTRA_ASTGEN=0)
  endif()
else()
  message("-- khuneopp > ASTGen unavailable!")
  target_compile_definitions(khuneopp PUBLIC KHPP_ASTGEN=0)
endif ()
