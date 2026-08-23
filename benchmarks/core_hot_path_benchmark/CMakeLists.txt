add_executable(kke_core_hot_path_benchmark
    Main.cc
    AllocationTracker.cc
    CoreHotPathBenchmark.cc
)

target_include_directories(
    kke_core_hot_path_benchmark
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(kke_core_hot_path_benchmark PRIVATE kke::kke)
