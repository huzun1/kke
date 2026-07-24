cmake_minimum_required(VERSION 3.10.0)
project(kke_test CXX)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/resources/resources.rc
    @ONLY
)

set(KKE_TEST_RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources/resources.res)

add_custom_command(
    OUTPUT ${KKE_TEST_RESOURCE_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_RC_COMPILER} /nologo /fo ${KKE_TEST_RESOURCE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/resources/resources.rc
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/resources/resources.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.h
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/contents/dycontrast_1337.png
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/contents/SpaceGrotesk-VariableFont_wght.ttf
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/contents/mountain.png
    VERBATIM
)

add_executable(kke_test WIN32
    Main.cc
    Window.cc
    Renderer.cc
    renderer_test/RendererTest.cc
    renderer_test/BlurStressRendererTest.cc
    renderer_test/BlurStressBackgroundRendererTest.cc
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
    ${KKE_TEST_RESOURCE_FILE}
 )

set_source_files_properties(${KKE_TEST_RESOURCE_FILE} PROPERTIES
    EXTERNAL_OBJECT TRUE
)

target_link_libraries(kke_test kke)
target_include_directories(kke_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(kke_matrix_test MatrixTest.cc)
target_link_libraries(kke_matrix_test PRIVATE kke)
add_test(NAME kke_matrix_test COMMAND kke_matrix_test)
