#pragma once

#include <cstddef>
#include <cstdint>

namespace kke::benchmark {
struct ShadowFrameSample {
	double recordMilliseconds;
	double submitMilliseconds;
	double gpuElapsedMilliseconds;
	double completedMilliseconds;
	uint64_t hits;
	uint64_t misses;
	std::size_t geometricLayers;
	std::size_t axisClips;
	std::size_t bitmapDraws;
};
} // namespace kke::benchmark
