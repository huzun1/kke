cmake_minimum_required(VERSION 3.10.0)
project(kke CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(kke STATIC
    src/kke/appearance/Color.cc
    src/kke/appearance/Scale.cc
    src/kke/appearance/resource/GeometryCompose.cc
    src/kke/appearance/resource/brush/impl/LinearGradientBrush.cc
    src/kke/appearance/resource/brush/impl/SolidColorBrush.cc
    src/kke/engine/d2d/D2dEngine.cc
    src/kke/engine/d2d/context/D2dContext.cc
    src/kke/engine/d2d/context/D2dEngineContext.cc
    src/kke/engine/d2d/context/D2dResourceProviders.cc
    src/kke/engine/d2d/renderer/Measurer.cc
    src/kke/engine/d2d/renderer/RenderPass.cc
    src/kke/engine/d2d/renderer/canvas/CanvasRenderTargetStack.cc
    src/kke/engine/d2d/renderer/canvas/CanvasService.cc
    src/kke/engine/d2d/renderer/painting/FaceRenderer.cc
    src/kke/engine/d2d/renderer/painting/StrokeRenderer.cc
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

target_include_directories(kke PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(kke PUBLIC dwrite)

install(TARGETS kke
    EXPORT kkeTargets
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
    DESTINATION include
)

add_subdirectory(test)
