add_executable(kke_raw_d2d_render_pass_benchmark
    Main.cc
    RawD2dRenderPassBenchmark.cc
)

target_include_directories(
    kke_raw_d2d_render_pass_benchmark
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(
    kke_raw_d2d_render_pass_benchmark
    PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX
)

target_link_libraries(
    kke_raw_d2d_render_pass_benchmark
    PRIVATE d2d1 dwrite d3d11 dxgi dxguid user32
)
