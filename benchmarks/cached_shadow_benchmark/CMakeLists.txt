add_executable(kke_cached_shadow_benchmark
    Main.cc
    ShadowBenchmarkDevice.cc
    CachedShadowBenchmark.cc
    ShadowRasterization.cc
    ShadowQualityReport.cc
)
target_link_libraries(kke_cached_shadow_benchmark PRIVATE kke::kke d3d11 dxgi)

# Override this static-library translation unit only in the benchmark executable.
set(shadow_renderer_dir "${PROJECT_SOURCE_DIR}/src/kke/engine/d2d/renderer/effect/renderers")
set(shadow_renderer_source "${shadow_renderer_dir}/ShadowEffectRenderer.cc")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${shadow_renderer_source}")
file(READ "${shadow_renderer_source}" shadow_renderer_code)
string(REGEX MATCHALL "if \\(cache.supports\\(source\\)\\)" cache_branches "${shadow_renderer_code}")
list(LENGTH cache_branches cache_branch_count)
if(NOT cache_branch_count EQUAL 1)
    message(FATAL_ERROR "Shadow benchmark override must match exactly one cache branch")
endif()
string(REPLACE "if (cache.supports(source))"
    "if (kke::benchmark::ShadowCacheControl::isEnabled() && cache.supports(source))"
    shadow_renderer_code "${shadow_renderer_code}")
set(shadow_renderer_override "${CMAKE_CURRENT_BINARY_DIR}/ShadowEffectRenderer.cc")
file(WRITE "${shadow_renderer_override}"
    "#include \"ShadowCacheControl.hh\"\n${shadow_renderer_code}")
target_sources(kke_cached_shadow_benchmark PRIVATE "${shadow_renderer_override}")
target_include_directories(kke_cached_shadow_benchmark PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}" "${shadow_renderer_dir}")

set(effect_renderer_dir "${PROJECT_SOURCE_DIR}/src/kke/engine/d2d/renderer/effect")
set(effect_renderer_source "${effect_renderer_dir}/EffectRenderer.cc")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${effect_renderer_source}")
file(READ "${effect_renderer_source}" effect_renderer_code)
string(REPLACE "\r\n" "\n" effect_renderer_code "${effect_renderer_code}")
set(offset_draw "tryDrawBitmap(context, result.image, result.targetOffset)")
string(FIND "${effect_renderer_code}" "${offset_draw}" offset_draw_index)
if(offset_draw_index EQUAL -1)
    message(FATAL_ERROR "Shadow bitmap benchmark override cannot find bitmap draw")
endif()
string(REPLACE "${offset_draw}" "" effect_renderer_without_draw "${effect_renderer_code}")
string(LENGTH "${effect_renderer_code}" original_length)
string(LENGTH "${effect_renderer_without_draw}" replaced_length)
string(LENGTH "${offset_draw}" draw_length)
math(EXPR removed_length "${original_length} - ${replaced_length}")
if(NOT removed_length EQUAL draw_length)
    message(FATAL_ERROR "Shadow bitmap benchmark override must match exactly one draw")
endif()
string(REPLACE "${offset_draw}"
    "(kke::benchmark::ShadowBitmapDrawing::isEnabled() && kke::benchmark::ShadowBitmapDrawing::recordDraw(${offset_draw}))"
    effect_renderer_code "${effect_renderer_code}")
set(effect_renderer_override "${CMAKE_CURRENT_BINARY_DIR}/EffectRenderer.cc")
file(WRITE "${effect_renderer_override}"
    "#include \"ShadowBitmapDrawing.hh\"\n${effect_renderer_code}")
target_sources(kke_cached_shadow_benchmark PRIVATE "${effect_renderer_override}")
target_include_directories(kke_cached_shadow_benchmark PRIVATE "${effect_renderer_dir}")

set(cache_source "${shadow_renderer_dir}/PositionIndependentEffectCache.cc")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${cache_source}")
file(READ "${cache_source}" cache_code)
set(cache_create "createEffectBitmap(context, outputImage, imageBounds)")
set(cache_return "return {bitmap, {imageBounds.left, imageBounds.top}, estimateBitmapByteSize(bitmap.Get()), 0};")
foreach(needle IN ITEMS "${cache_create}" "${cache_return}")
    string(REPLACE "${needle}" "" remaining "${cache_code}")
    string(LENGTH "${cache_code}" original_length)
    string(LENGTH "${remaining}" remaining_length)
    string(LENGTH "${needle}" needle_length)
    math(EXPR removed_length "${original_length} - ${remaining_length}")
    if(NOT removed_length EQUAL needle_length)
        message(FATAL_ERROR "Shadow resolution benchmark override must match exactly once: ${needle}")
    endif()
endforeach()
string(REPLACE "${cache_create}"
    "(kke::benchmark::ShadowRasterization::scale() == 1.0f ? ${cache_create} : kke::benchmark::ShadowRasterization::create(context, outputImage, imageBounds))"
    cache_code "${cache_code}")
string(REPLACE "${cache_return}"
    "kke::benchmark::ShadowRasterization::recordBitmap(bitmap.Get());\n\t${cache_return}"
    cache_code "${cache_code}")
set(cache_override "${CMAKE_CURRENT_BINARY_DIR}/PositionIndependentEffectCache.cc")
file(WRITE "${cache_override}" "#include \"ShadowRasterization.hh\"\n${cache_code}")
target_sources(kke_cached_shadow_benchmark PRIVATE "${cache_override}")
