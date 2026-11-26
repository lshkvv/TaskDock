cmake_minimum_required(VERSION 3.20)
project(taskdock LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(taskdock
  src/main.cpp
  src/process/process_runner.cpp
  src/suite.cpp
)
if(WIN32)
  target_compile_definitions(taskdock PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
  target_link_libraries(taskdock PRIVATE kernel32)
endif()
