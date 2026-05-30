cmake_minimum_required(VERSION 3.10.0)
project(kke_test CXX)

add_executable(kke_test WIN32
    Main.cc
    Window.cc
    Renderer.cc
    renderer_test/RendererTest.cc
    renderer_test/BlurStressRendererTest.cc
    renderer_test/LayerRendererTest.cc
    renderer_test/ShadowStressRendererTest.cc
    renderer_test/ShapeEffectRendererTest.cc
    renderer_test/CanvasRendererTest.cc
    renderer_test/TextureRendererTest.cc
    renderer_test/TextEffectRendererTest.cc
    renderer_test/FrameEffectRendererTest.cc
    renderer_test/TransformRendererTest.cc
    backend/D2D1.cc
    backend/D3D11.cc
    resources/resources.h
    resources/resources.rc
 )
target_link_libraries(kke_test kke)
target_include_directories(kke_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
