cmake_minimum_required(VERSION 3.10.0)

add_library(kke STATIC
    Engine.cc
    ResourceAllocator.cc
    TextureRepository.cc
    RenderSurface.cc
    ShadowDispatcher.cc
    common/Point.cc
    common/Color.cc
    common/geometry/Ellipse.cc
    common/geometry/Rect.cc
    common/geometry/RoundedRect.cc
    common/geometry/Triangle.cc
    brush/SolidColorBrush.cc
    brush/LinearGradientBrush.cc
    effect/EffectContainer.cc
    effect/Effect.cc
    effect/EffectInstance.cc
    effect/impl/BlurEffect.cc
    transform/impl/RotateTransform.cc
    transform/impl/ScaleTransform.cc
    transform/Matrix.cc
    font/FontLoader.cc
)

target_include_directories(kke PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../includes>
    $<INSTALL_INTERFACE:include>
)
install(TARGETS kke
    EXPORT kkeTargets
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
    DESTINATION include
)
