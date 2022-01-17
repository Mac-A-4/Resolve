cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(Resolve)

add_library(Resolve STATIC src/Resolve.cpp)

target_include_directories(Resolve PUBLIC include)

target_link_libraries(Resolve PRIVATE GetProcAddressEx)

add_executable(ResolveTest src/Main.cpp)

target_link_libraries(ResolveTest PRIVATE Resolve)