cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (Main)

set(RESOURCE_FOLDER "C:\\repos\\Pixal\\source\\resources")

include_directories(source)
link_directories("C:\\repos\\pixal\\source\\thirdparty\\glfw3" "C:\\repos\\pixal\\source\\thirdparty\\glad")

add_subdirectory(source)
add_subdirectory(test/triangle)