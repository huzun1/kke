cmake_minimum_required(VERSION 3.10.0)
project(kke_test CXX)

add_executable(kke_test WIN32
    Main.cc
    Window.cc
    Renderer.cc
    backend/D2D1.cc
    backend/D3D11.cc
    resources/resources.h
    resources/resources.rc
 )
target_link_libraries(kke_test kke yogacore)
target_include_directories(kke_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
