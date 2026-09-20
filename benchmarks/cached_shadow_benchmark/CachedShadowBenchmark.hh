#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

#include "ShadowBenchmarkDevice.hh"
#include "ShadowFrameSample.hh"
#include "ShadowScenario.hh"
#include "kke/engine/d2d/D2dEngine.hh"

namespace kke::benchmark {
class CachedShadowBenchmark {
	ShadowBenchmarkDevice device;
	std::size_t samples;
	std::size_t rounds;
	std::string_view comparison;

  public:
	CachedShadowBenchmark(
		std::size_t samples, std::size_t rounds, std::string_view comparison = {}
	);

	bool run();

  private:
	bool verifyBitmapDrawing();

	bool compareResolutionQuality();

	bool runScenario(std::size_t round, std::size_t panels, ShadowScenario mode);

	static void configureScenario(ShadowScenario mode);

	std::optional<ShadowFrameSample>
	renderFrame(D2dEngine& engine, std::size_t panels, ShadowScenario mode, std::size_t frame);

	void drawPanels(D2dEngine& engine, std::size_t panels, ShadowScenario mode, std::size_t frame);

	static void report(
		std::size_t round,
		std::size_t panels,
		ShadowScenario mode,
		std::span<ShadowFrameSample const> timings
	);

	static std::string_view modeName(ShadowScenario mode);
};
} // namespace kke::benchmark
