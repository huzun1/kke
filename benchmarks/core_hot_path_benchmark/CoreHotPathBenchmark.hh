#pragma once

#include <chrono>
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

#include "AllocationTracker.hh"
#include "kke/appearance/Text.hh"

namespace kke::benchmark {
class CoreHotPathBenchmark {
	using Clock = std::chrono::steady_clock;

	struct Sample {
		double microseconds = 0.0;
		AllocationSample allocations;
	};

	struct Distribution {
		double average = 0.0;
		double p50 = 0.0;
		double p95 = 0.0;
	};

	static constexpr size_t textCount = 38;

	size_t measuredIterationCount;

	static double elapsedMicroseconds(Clock::time_point begin, Clock::time_point end);

	static Distribution distribution(std::span<Sample const> samples);

	static void printScenario(
		std::string_view name, std::span<Sample const> samples, size_t operationsPerSample
	);

	std::vector<Sample> measureTextHash() const;

	std::vector<Sample> measureContextAttachRelease() const;

  public:
	explicit CoreHotPathBenchmark(size_t measuredIterationCount);

	int run() const;
};
} // namespace kke::benchmark
