cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXE modelview)
project (${EXE})

add_executable(${EXE} main.cpp game.h game.cpp)

target_link_libraries(${EXE} game)

add_custom_command(
  TARGET 
    ${EXE} POST_BUILD
  DEPENDS
    ${RESOURCE_FOLDER}
  COMMAND 
    ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_FOLDER} ${PROJECT_BINARY_DIR}
  COMMENT 
    "copy resource folder ${RESOURCE_FOLDER} to ${PROJECT_BINARY_DIR}"
)