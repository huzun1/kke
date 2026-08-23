#include "CoreHotPathBenchmark.hh"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/resource/font/hash/FontHasher.hh"

using namespace kke::benchmark;

double CoreHotPathBenchmark::elapsedMicroseconds(Clock::time_point begin, Clock::time_point end) {
	return std::chrono::duration<double, std::micro>(end - begin).count();
}

CoreHotPathBenchmark::Distribution
CoreHotPathBenchmark::distribution(std::span<Sample const> samples) {
	std::vector<double> values;
	values.reserve(samples.size());
	for (Sample const& sample : samples) {
		values.push_back(sample.microseconds);
	}
	std::sort(values.begin(), values.end());
	double sum = std::accumulate(values.begin(), values.end(), 0.0);
	size_t p95Index = static_cast<size_t>(std::ceil(values.size() * 0.95)) - 1;
	return {
		.average = sum / static_cast<double>(values.size()),
		.p50 = values[values.size() / 2],
		.p95 = values[std::min(p95Index, values.size() - 1)],
	};
}

void CoreHotPathBenchmark::printScenario(
	std::string_view name, std::span<Sample const> samples, size_t operationsPerSample
) {
	Distribution timing = distribution(samples);
	size_t allocationCount = 0;
	size_t allocatedBytes = 0;
	for (Sample const& sample : samples) {
		allocationCount += sample.allocations.count;
		allocatedBytes += sample.allocations.bytes;
	}
	double operationCount = static_cast<double>(samples.size() * operationsPerSample);

	std::cout << std::left << std::setw(28) << name << std::right << std::fixed
			  << std::setprecision(3) << std::setw(11) << timing.average << std::setw(11)
			  << timing.p50 << std::setw(11) << timing.p95 << std::setw(13)
			  << static_cast<double>(allocationCount) / operationCount << std::setw(13)
			  << static_cast<double>(allocatedBytes) / operationCount << '\n';
}

std::vector<CoreHotPathBenchmark::Sample> CoreHotPathBenchmark::measureTextHash() const {
	std::vector<::kke::Text> texts;
	texts.reserve(textCount);
	for (size_t index = 0; index < textCount; ++index) {
		texts.push_back({
			.text = std::wstring(L"Array List Entry ") + std::to_wstring(index),
			.position = {0.0f, 0.0f},
			.fontAppearance =
				{
					.fontFamily = "Inter",
					.fontSize = 14.0f,
				},
		});
	}

	std::vector<Sample> samples;
	samples.reserve(measuredIterationCount);
	volatile uint64_t hashSink = 0;
	for (size_t iteration = 0; iteration < measuredIterationCount; ++iteration) {
		AllocationTracker::begin();
		auto begin = Clock::now();
		uint64_t combinedHash = 0;
		for (::kke::Text const& text : texts) {
			combinedHash ^= ::kke::FontHasher::hash(text);
		}
		auto end = Clock::now();
		AllocationSample allocations = AllocationTracker::finish();
		hashSink = hashSink ^ combinedHash;
		samples.push_back({elapsedMicroseconds(begin, end), allocations});
	}
	(void)hashSink;
	return samples;
}

std::vector<CoreHotPathBenchmark::Sample>
CoreHotPathBenchmark::measureContextAttachRelease() const {
	::kke::D2dEngineContext engineContext;
	::kke::D2dContext drawContext(nullptr, nullptr);
	std::vector<Sample> samples;
	samples.reserve(measuredIterationCount);
	for (size_t iteration = 0; iteration < measuredIterationCount; ++iteration) {
		AllocationTracker::begin();
		auto begin = Clock::now();
		engineContext.setD2dContext(drawContext);
		engineContext.releaseD2dContext();
		auto end = Clock::now();
		AllocationSample allocations = AllocationTracker::finish();
		samples.push_back({elapsedMicroseconds(begin, end), allocations});
	}
	return samples;
}

CoreHotPathBenchmark::CoreHotPathBenchmark(size_t measuredIterationCount)
	: measuredIterationCount(measuredIterationCount) {
}

int CoreHotPathBenchmark::run() const {
	std::cout << "KKE core hot-path microbenchmark: " << measuredIterationCount
			  << " measured iterations\n\n";
	std::cout << std::left << std::setw(28) << "scenario" << std::right << std::setw(11) << "avg-us"
			  << std::setw(11) << "p50-us" << std::setw(11) << "p95-us" << std::setw(13)
			  << "allocs/op" << std::setw(13) << "bytes/op" << '\n';

	auto textHashSamples = measureTextHash();
	printScenario("text-hash/38-wide", textHashSamples, textCount);
	auto contextSamples = measureContextAttachRelease();
	printScenario("context/attach-release", contextSamples, 1);
	return 0;
}
