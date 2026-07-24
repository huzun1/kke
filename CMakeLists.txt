cmake_minimum_required(VERSION 3.10.0)
project(kke CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(KKE_IS_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(KKE_IS_TOP_LEVEL ON)
endif()

option(KKE_BUILD_TESTS "Build the kke test application" ${KKE_IS_TOP_LEVEL})
option(KKE_DEBUG "Enable kke debug logging" ${KKE_IS_TOP_LEVEL})

set(KKE_PUBLIC_ENGINE_HEADERS
    src/kke/engine/Engine.hh
    src/kke/engine/Sources.hh
)

set(KKE_PUBLIC_D2D_HEADERS
    src/kke/engine/d2d/D2dEngine.hh
)

set(KKE_PUBLIC_D2D_CONTEXT_HEADERS
    src/kke/engine/d2d/context/D2dContext.hh
)

add_library(kke STATIC
    src/kke/appearance/Color.cc
    src/kke/appearance/ColorQuantizer.cc
    src/kke/appearance/Scale.cc
    src/kke/appearance/resource/GeometryCompose.cc
    src/kke/appearance/resource/effect/EffectCompose.cc
    src/kke/appearance/resource/effect/EffectIdentifier.cc
    src/kke/appearance/resource/brush/impl/LinearGradientBrush.cc
    src/kke/appearance/resource/brush/impl/SolidColorBrush.cc
    src/kke/engine/d2d/D2dEngine.cc
    src/kke/engine/d2d/context/D2dContext.cc
    src/kke/engine/d2d/context/D2dEngineContext.cc
    src/kke/engine/d2d/context/D2dResourceProviders.cc
    src/kke/engine/d2d/renderer/CommandListSnapshotter.cc
    src/kke/engine/d2d/renderer/Measurer.cc
    src/kke/engine/d2d/renderer/RenderPass.cc
    src/kke/engine/d2d/renderer/canvas/CanvasRenderTargetStack.cc
    src/kke/engine/d2d/renderer/canvas/CanvasService.cc
    src/kke/engine/d2d/renderer/effect/EffectClipBoundsResolver.cc
    src/kke/engine/d2d/renderer/effect/EffectClipCropper.cc
    src/kke/engine/d2d/renderer/effect/EffectClipTransformer.cc
    src/kke/engine/d2d/renderer/effect/EffectPaddingEstimator.cc
    src/kke/engine/d2d/renderer/effect/EffectRenderer.cc
    src/kke/engine/d2d/renderer/effect/EffectSourceRenderer.cc
    src/kke/engine/d2d/renderer/effect/cache/PositionIndependentBrush.cc
    src/kke/engine/d2d/renderer/effect/cache/PositionIndependentEffectSource.cc
    src/kke/engine/d2d/renderer/effect/renderers/BlurEffectRenderer.cc
    src/kke/engine/d2d/renderer/effect/renderers/ColorMatrixEffectRenderer.cc
    src/kke/engine/d2d/renderer/effect/renderers/DirectionalBlurEffectRenderer.cc
    src/kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.cc
    src/kke/engine/d2d/renderer/effect/renderers/ShadowEffectRenderer.cc
    src/kke/engine/d2d/renderer/painting/FaceRenderer.cc
    src/kke/engine/d2d/renderer/painting/StrokeRenderer.cc
    src/kke/engine/d2d/renderer/painting/TextureRenderer.cc
    src/kke/engine/d2d/renderer/view/MatrixState.cc
    src/kke/engine/d2d/renderer/view/ViewLayerController.cc
    src/kke/engine/d2d/resource/brush/BrushProvider.cc
    src/kke/engine/d2d/resource/brush/factory/BrushFactory.cc
    src/kke/engine/d2d/resource/brush/hash/BrushHasher.cc
    src/kke/engine/d2d/resource/font/D2dFont.cc
    src/kke/engine/d2d/resource/font/FontProvider.cc
    src/kke/engine/d2d/resource/font/TextFormatProvider.cc
    src/kke/engine/d2d/resource/font/TextLayoutProvider.cc
    src/kke/engine/d2d/resource/font/factory/FontFactory.cc
    src/kke/engine/d2d/resource/font/hash/FontHasher.cc
    src/kke/engine/d2d/resource/canvas/CanvasFactory.cc
    src/kke/engine/d2d/resource/canvas/D2dCanvas.cc
    src/kke/engine/d2d/resource/geometry/GeometryProvider.cc
    src/kke/engine/d2d/resource/geometry/InvertedGeometryProvider.cc
    src/kke/engine/d2d/resource/geometry/ViewportGeometryCache.cc
    src/kke/engine/d2d/resource/geometry/factory/GeometryFactory.cc
    src/kke/engine/d2d/resource/geometry/factory/invert/GeometryInterver.cc
    src/kke/engine/d2d/resource/geometry/factory/invert/InvertedGeometryFactory.cc
    src/kke/engine/d2d/resource/geometry/hash/GeometryHasher.cc
    src/kke/engine/d2d/resource/texture/D2dTexture.cc
    src/kke/engine/d2d/resource/texture/TextureProvider.cc
    src/kke/engine/d2d/resource/texture/factory/TextureFactory.cc
    src/kke/engine/d2d/transform/Matrix.cc
    src/kke/engine/d2d/transform/impl/RotateManipulator.cc
    src/kke/engine/d2d/transform/impl/ScaleManipulator.cc
    src/kke/engine/d2d/transform/impl/TranslateManipulator.cc
    src/kke/geometry/Boundary.cc
    src/kke/geometry/curved/Ellipse.cc
    src/kke/geometry/curved/EllipseOperators.cc
    src/kke/geometry/curved/RoundedRect.cc
    src/kke/geometry/primitives/Line.cc
    src/kke/geometry/primitives/Point.cc
    src/kke/geometry/primitives/PointOperators.cc
    src/kke/geometry/shapes/Polygon.cc
    src/kke/geometry/shapes/Rect.cc
    src/kke/geometry/shapes/RectOperators.cc
    src/kke/geometry/shapes/Triangle.cc
    src/kke/geometry/shapes/TriangleOperators.cc
)

add_library(kke::kke ALIAS kke)

target_include_directories(kke PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(kke PUBLIC cxx_std_20)

target_compile_definitions(kke PRIVATE
    $<$<BOOL:${KKE_DEBUG}>:KKE_DEBUG=1>
)

target_link_libraries(kke PUBLIC d2d1 dwrite windowscodecs dxguid)

install(TARGETS kke
    EXPORT kkeTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kke/appearance
    ${CMAKE_CURRENT_SOURCE_DIR}/src/kke/geometry
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kke
    FILES_MATCHING
    PATTERN "*.hh"
)

install(FILES ${KKE_PUBLIC_ENGINE_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kke/engine
)

install(FILES ${KKE_PUBLIC_D2D_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kke/engine/d2d
)

install(FILES ${KKE_PUBLIC_D2D_CONTEXT_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kke/engine/d2d/context
)

configure_package_config_file(
    cmake/kkeConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/kkeConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kke
)

install(EXPORT kkeTargets
    FILE kkeTargets.cmake
    NAMESPACE kke::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kke
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kkeConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kke
)

if(KKE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
