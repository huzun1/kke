if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "kke_overlay_example requires a 64-bit toolchain")
endif()

include(FetchContent)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG 565968b28583221751cc2810e09ea621745fc3a3
)
FetchContent_MakeAvailable(minhook)

add_library(kiero STATIC third_party/kiero/Kiero.cc)
target_include_directories(kiero PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/kiero)
target_link_libraries(kiero PRIVATE d3d11 dxgi user32)

add_library(kke_overlay_example SHARED
    D2dEntrance.cc
    D3d11ContextIsolation.cc
    DllMain.cc
    Hooks.cc
    PageController.cc
    pages/FillPage.cc
    pages/FullBlurPage.cc
    pages/ShadowAndClippingPage.cc
    pages/TransformPage.cc
)

target_compile_features(kke_overlay_example PRIVATE cxx_std_23)
target_include_directories(kke_overlay_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kke_overlay_example PRIVATE
    kke::kke
    kiero
    minhook
    d2d1
    d3d11
    dxgi
)
