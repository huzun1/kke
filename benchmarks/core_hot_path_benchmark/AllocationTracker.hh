#pragma once

#include <cstddef>

namespace kke::benchmark {
struct AllocationSample {
	size_t count = 0;
	size_t bytes = 0;
};

class AllocationTracker {
	static thread_local bool isTracking;
	static thread_local AllocationSample currentSample;

  public:
	static void begin();

	static AllocationSample finish();

	static void record(size_t bytes);
};
} // namespace kke::benchmark
