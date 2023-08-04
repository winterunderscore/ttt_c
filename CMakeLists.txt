cmake_minimum_required(VERSION 3.20)
project(ttc VERSION 0.0.1 LANGUAGES C)

add_executable(ttc)

target_sources(ttc
  PRIVATE src/main.c
)

target_compile_features(ttc
  PRIVATE c_std_99
)

target_compile_options(ttc
  PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -O3
  -g
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(ttc
    PRIVATE
    -fsanitize=address,undefined
  )

  target_link_options(ttc
    PRIVATE
    -fsanitize=address,undefined
  )
endif()

add_custom_command(TARGET ttc
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different $<TARGET_FILE:ttc> ${CMAKE_SOURCE_DIR}/bin
)