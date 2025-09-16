cmake_minimum_required(VERSION 3.10.0)

add_library(oreik STATIC
    Engine.cc
    ResourceAllocator.cc
    common/Ellipse.cc
    common/Point.cc
    common/Rect.cc
    brush/SolidColorBrush.cc
)

target_include_directories(oreik PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../includes>
    $<INSTALL_INTERFACE:include>
)
install(TARGETS oreik
    EXPORT oreikTargets
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
    DESTINATION include
)
