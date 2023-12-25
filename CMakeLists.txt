cmake_minimum_required(VERSION 3.16)
project(Compressor VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRCS src/*.c src/*.cpp src/*.h)

add_executable(Compressor ${SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_EXE_LINKER_FLAGS " -static")

target_link_libraries(Compressor -static-libgcc -static-libstdc++)