cmake_minimum_required(VERSION 3.10.0)
project(oreik_test CXX)

add_executable(oreik_test WIN32
    Main.cc
    Window.cc
    Renderer.cc
    backend/D2D1.cc
    backend/D3D11.cc
 )
target_link_libraries(oreik_test oreik)
target_include_directories(oreik_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})