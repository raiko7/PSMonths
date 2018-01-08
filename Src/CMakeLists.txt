cmake_minimum_required(VERSION 3.1)
project(PSMonths)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Srcs PSMonths.cpp Formatter.cpp DateUtils.cpp)
add_executable(PSMonths ${Srcs})

install(TARGETS PSMonths DESTINATION bin)
