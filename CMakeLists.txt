cmake_minimum_required(VERSION 3.10.0)
project(kke CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch Yoga layout engine
include(FetchContent)
FetchContent_Declare(
    yoga
    GIT_REPOSITORY https://github.com/facebook/yoga.git
    GIT_TAG v3.1.0
)
FetchContent_MakeAvailable(yoga)

add_subdirectory(src)
add_subdirectory(test)
