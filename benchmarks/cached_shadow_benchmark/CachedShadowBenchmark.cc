#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "CachedShadowBenchmark.hh"
#include "ShadowBitmapDrawing.hh"
#include "ShadowCacheControl.hh"
#include "ShadowQualityReport.hh"
#include "ShadowRasterization.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/transform/AffineTransform.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"
#include "kke/geometry/curved/RoundedRect.hh"

using namespace kke;
using namespace kke::benchmark;

CachedShadowBenchmark::CachedShadowBenchmark(
	std::size_t samples, std::size_t rounds, std::string_view comparison
)
	: samples(samples), rounds(rounds), comparison(comparison) {
}

bool CachedShadowBenchmark::run() {
	if (!device.initialize()) {
		return false;
	}
	if (comparison == "resolution" ? !compareResolutionQuality() : !verifyBitmapDrawing()) {
		return false;
	}
	std::printf(
		"samples=%zu rounds=%zu warmup=60 radius=8 shadow_sigma=8 offset_y=4 alpha=0.52\n",
		samples,
		rounds
	);
	std::printf("GPU elapsed includes submission gaps; completed includes query wait. No timed "
				"readback or Present.\n");
	std::printf("round,panels,mode,record_median_ms,record_p95_ms,submit_median_ms,submit_p95_ms,"
				"cpu_median_ms,cpu_p95_ms,gpu_elapsed_median_ms,gpu_elapsed_p95_ms,completed_"
				"median_ms,completed_p95_ms,hits,misses,layers,axis_clips,bitmap_draws,generated_"
				"bitmap_bytes,generated_bitmaps\n");
	constexpr std::array allScenarios{
		ShadowScenario::NoShadow,
		ShadowScenario::Cached,
		ShadowScenario::CacheMiss,
		ShadowScenario::CachedWithoutClip,
		ShadowScenario::Disabled,
		ShadowScenario::CachedBitmap
	};
	constexpr std::array bitmapScenarios{ShadowScenario::Cached, ShadowScenario::CachedBitmap};
	constexpr std::array resolutionScenarios{
		ShadowScenario::CachedBitmap,
		ShadowScenario::HalfResolution,
		ShadowScenario::QuarterResolution
	};
	std::span<ShadowScenario const> scenarios =
		comparison == "resolution" ? std::span<ShadowScenario const>(resolutionScenarios)
		: comparison == "bitmap"   ? std::span<ShadowScenario const>(bitmapScenarios)
								   : std::span<ShadowScenario const>(allScenarios);
	for (std::size_t round = 0; round < rounds; ++round) {
		for (std::size_t panels : {1u, 6u, 24u}) {
			for (std::size_t order = 0; order < scenarios.size(); ++order) {
				ShadowScenario mode = scenarios[(order + round) % scenarios.size()];
				if (!runScenario(round, panels, mode)) {
					return false;
				}
			}
		}
	}
	return true;
}

bool CachedShadowBenchmark::verifyBitmapDrawing() {
	ShadowCacheControl::setEnabled(true);
	std::array<AffineTransform, 4> transforms{
		AffineTransform{},
		AffineTransform{.tx = 0.375f, .ty = 0.625f},
		AffineTransform{.xx = 0.85f, .yy = 1.2f, .tx = 0.25f, .ty = 0.5f},
		AffineTransform{.xx = 0.98f, .xy = 0.2f, .yx = -0.2f, .yy = 0.98f, .tx = 200.0f}
	};
	int maximumDifference = 0;
	std::size_t differingChannels = 0;
	std::size_t cases = 0;
	for (std::size_t panels : {1u, 6u, 24u}) {
		D2dEngine engine;
		for (auto const& transform : transforms) {
			for (bool hasBackground : {false, true}) {
				for (bool hasPartialClip : {false, true}) {
					std::vector<uint8_t> reference;
					for (bool useBitmap : {false, true}) {
						ShadowBitmapDrawing::setEnabled(useBitmap);
						ShadowBitmapDrawing::consumeDrawCount();
						engine.beginDraw(device.drawingContext(), device.bitmap());
						engine.clear();
						if (hasBackground) {
							engine.fill(
								Rect{{0, 0}, {1920, 1080}},
								SolidColorBrush({0.2f, 0.4f, 0.6f, 1.0f})
							);
						}
						if (hasPartialClip) {
							engine.pushLayer(Rect{{48, 44}, {1800, 650}});
						}
						engine.pushTransform(transform);
						drawPanels(engine, panels, ShadowScenario::Cached, 0);
						engine.popTransform();
						if (hasPartialClip) {
							engine.popLayer();
						}
						engine.endDraw();
						auto pixels = device.readPixels();
						if (!pixels ||
							ShadowBitmapDrawing::consumeDrawCount() != (useBitmap ? panels : 0)) {
							std::fprintf(
								stderr,
								"Bitmap image comparison readback/draw count failed.\n"
							);
							return false;
						}
						if (!useBitmap) {
							reference = std::move(*pixels);
							continue;
						}
						for (std::size_t index = 0; index < reference.size(); ++index) {
							int difference =
								std::abs(int(reference[index]) - int((*pixels)[index]));
							maximumDifference = (std::max)(maximumDifference, difference);
							differingChannels += difference != 0;
						}
						++cases;
					}
				}
			}
		}
	}
	std::fprintf(
		stderr,
		"DrawImage/DrawBitmap pixel comparison: cases=%zu max_channel_delta=%d "
		"differing_channels=%zu tolerance=1/255\n",
		cases,
		maximumDifference,
		differingChannels
	);
	return maximumDifference <= 1;
}

bool CachedShadowBenchmark::compareResolutionQuality() {
	std::array<AffineTransform, 3> transforms{
		AffineTransform{},
		AffineTransform{.tx = 0.375f, .ty = 0.625f},
		AffineTransform{.xx = 1.25f, .yy = 0.85f, .tx = 0.25f, .ty = 0.5f}
	};
	std::array<std::string_view, 3> names{"identity", "fractional", "scaled"};
	for (std::size_t index = 0; index < transforms.size(); ++index) {
		for (bool hasBackground : {false, true}) {
			std::vector<uint8_t> reference;
			for (auto mode :
				 {ShadowScenario::CachedBitmap,
				  ShadowScenario::HalfResolution,
				  ShadowScenario::QuarterResolution}) {
				configureScenario(mode);
				D2dEngine engine;
				std::optional<std::vector<uint8_t>> pixels;
				for (int frame = 0; frame < 2; ++frame) {
					PositionIndependentEffectCache::consumeStats();
					ShadowBitmapDrawing::consumeDrawCount();
					engine.beginDraw(device.drawingContext(), device.bitmap());
					engine.clear();
					if (hasBackground) {
						engine.fill(
							Rect{{0, 0}, {1920, 1080}},
							SolidColorBrush({0.35f, 0.45f, 0.6f, 1.0f})
						);
					}
					engine.pushTransform(transforms[index]);
					drawPanels(engine, 1, mode, 0);
					engine.popTransform();
					engine.endDraw();
					pixels = device.readPixels();
					auto stats = PositionIndependentEffectCache::consumeStats();
					if (!pixels || ShadowBitmapDrawing::consumeDrawCount() != 1 ||
						stats.hits != frame || stats.misses != 1 - frame) {
						return false;
					}
				}
				if (mode == ShadowScenario::CachedBitmap) {
					reference = *pixels;
				}
				std::string scene =
					std::string(names[index]) + (hasBackground ? "-opaque" : "-transparent");
				if (!ShadowQualityReport::write(reference, *pixels, scene, modeName(mode))) {
					return false;
				}
			}
		}
	}
	return true;
}

bool CachedShadowBenchmark::runScenario(
	std::size_t round, std::size_t panels, ShadowScenario mode
) {
	configureScenario(mode);
	D2dEngine engine;
	std::vector<ShadowFrameSample> timings;
	timings.reserve(samples);
	for (std::size_t frame = 0; frame < samples + 60; ++frame) {
		auto timing = renderFrame(engine, panels, mode, frame);
		if (!timing) {
			std::fprintf(
				stderr,
				"Invalid GPU timing or cache counts, mode=%d frame=%zu.\n",
				static_cast<int>(mode),
				frame
			);
			return false;
		}
		if (frame == 0 && !device.hasShadowPixels(mode != ShadowScenario::NoShadow)) {
			std::fprintf(stderr, "Shadow pixel sanity check failed.\n");
			return false;
		}
		if (frame >= 60) {
			timings.push_back(*timing);
		}
	}
	report(round, panels, mode, timings);
	return true;
}

void CachedShadowBenchmark::configureScenario(ShadowScenario mode) {
	ShadowCacheControl::setEnabled(mode != ShadowScenario::Disabled);
	ShadowBitmapDrawing::setEnabled(
		mode == ShadowScenario::CachedBitmap || mode == ShadowScenario::HalfResolution ||
		mode == ShadowScenario::QuarterResolution
	);
	ShadowRasterization::configure(
		mode == ShadowScenario::HalfResolution		? 0.5f
		: mode == ShadowScenario::QuarterResolution ? 0.25f
													: 1.0f
	);
}

std::optional<ShadowFrameSample> CachedShadowBenchmark::renderFrame(
	D2dEngine& engine, std::size_t panels, ShadowScenario mode, std::size_t frame
) {
	using Clock = std::chrono::steady_clock;
	PositionIndependentEffectCache::consumeStats();
	ShadowBitmapDrawing::consumeDrawCount();
	device.beginTiming();
	auto start = Clock::now();
	engine.beginDraw(device.drawingContext(), device.bitmap());
	engine.clear();
	drawPanels(engine, panels, mode, frame);
	auto recorded = Clock::now();
	engine.endDraw();
	auto submitted = Clock::now();
	device.endTiming();
	auto gpuMilliseconds = device.resolveMilliseconds();
	auto completed = Clock::now();
	auto stats = PositionIndependentEffectCache::consumeStats();
	auto bitmapDraws = ShadowBitmapDrawing::consumeDrawCount();
	bool isCachedScenario =
		mode == ShadowScenario::Cached || mode == ShadowScenario::CachedWithoutClip ||
		mode == ShadowScenario::CachedBitmap || mode == ShadowScenario::HalfResolution ||
		mode == ShadowScenario::QuarterResolution;
	uint64_t expectedMisses =
		mode == ShadowScenario::CacheMiss || (isCachedScenario && frame == 0) ? panels : 0;
	uint64_t expectedHits = isCachedScenario && frame != 0 ? panels : 0;
	if (!gpuMilliseconds || stats.hits != expectedHits || stats.misses != expectedMisses ||
		bitmapDraws != (ShadowBitmapDrawing::isEnabled() ? panels : 0) ||
		(isCachedScenario && ShadowRasterization::bitmapCount() != panels)) {
		std::fprintf(
			stderr,
			"hits=%llu misses=%llu expected=%llu/%llu\n",
			stats.hits,
			stats.misses,
			expectedHits,
			expectedMisses
		);
		return std::nullopt;
	}
	return ShadowFrameSample{
		std::chrono::duration<double, std::milli>(recorded - start).count(),
		std::chrono::duration<double, std::milli>(submitted - recorded).count(),
		*gpuMilliseconds,
		std::chrono::duration<double, std::milli>(completed - start).count(),
		stats.hits,
		stats.misses,
		engine.getLayerStatistics().geometricLayerCount,
		engine.getLayerStatistics().axisAlignedClipCount,
		bitmapDraws
	};
}

void CachedShadowBenchmark::drawPanels(
	D2dEngine& engine, std::size_t panels, ShadowScenario mode, std::size_t frame
) {
	for (std::size_t index = 0; index < panels; ++index) {
		float x = 32.0f + float(index % 6) * 300.0f;
		float y = 32.0f + float(index / 6) * 180.0f;
		RoundedRect shape({{x, y}, {x + 140.0f + float(index) * 2.0f, y + 36.0f}}, 8.0f);
		if (mode != ShadowScenario::NoShadow) {
			if (mode != ShadowScenario::CachedWithoutClip) {
				engine.pushLayer(Rect{{0, 0}, {1920, 1080}});
			}
			engine.renderEffect(
				Geometry(shape),
				{},
				ShadowEffect{
					.offset =
						{0,
						 4.0f + (mode == ShadowScenario::CacheMiss ? float(frame) * 0.0001f : 0.0f)
						},
					.blurStandardDeviation = 8.0f,
					.color = {0, 0, 0, 0.52f},
					.mode = ShadowMode::OuterShadowOnly
				}
			);
			if (mode != ShadowScenario::CachedWithoutClip) {
				engine.popLayer();
			}
		}
		engine.fill(shape, SolidColorBrush({0.08f, 0.09f, 0.1f, 0.8f}));
	}
}

void CachedShadowBenchmark::report(
	std::size_t round,
	std::size_t panels,
	ShadowScenario mode,
	std::span<ShadowFrameSample const> timings
) {
	std::array<std::vector<double>, 5> metrics;
	uint64_t hits = 0;
	uint64_t misses = 0;
	std::size_t layers = 0;
	std::size_t clips = 0;
	std::size_t bitmapDraws = 0;
	for (auto const& timing : timings) {
		metrics[0].push_back(timing.recordMilliseconds);
		metrics[1].push_back(timing.submitMilliseconds);
		metrics[2].push_back(timing.recordMilliseconds + timing.submitMilliseconds);
		metrics[3].push_back(timing.gpuElapsedMilliseconds);
		metrics[4].push_back(timing.completedMilliseconds);
		hits += timing.hits;
		misses += timing.misses;
		layers += timing.geometricLayers;
		clips += timing.axisClips;
		bitmapDraws += timing.bitmapDraws;
	}
	std::printf("%zu,%zu,%.*s", round, panels, int(modeName(mode).size()), modeName(mode).data());
	for (auto& values : metrics) {
		std::sort(values.begin(), values.end());
		std::printf(
			",%.6f,%.6f",
			values[values.size() / 2],
			values[(values.size() - 1) * 95 / 100]
		);
	}
	std::printf(
		",%llu,%llu,%zu,%zu,%zu,%zu,%zu\n",
		hits,
		misses,
		layers,
		clips,
		bitmapDraws,
		ShadowRasterization::bitmapBytes(),
		ShadowRasterization::bitmapCount()
	);
	std::fflush(stdout);
}

std::string_view CachedShadowBenchmark::modeName(ShadowScenario mode) {
	switch (mode) {
	case ShadowScenario::NoShadow:
		return "no_shadow";
	case ShadowScenario::Cached:
		return "cached_shadow";
	case ShadowScenario::CacheMiss:
		return "cache_miss";
	case ShadowScenario::CachedWithoutClip:
		return "cached_no_clip";
	case ShadowScenario::Disabled:
		return "cache_disabled";
	case ShadowScenario::CachedBitmap:
		return "cached_bitmap";
	case ShadowScenario::HalfResolution:
		return "half_resolution";
	case ShadowScenario::QuarterResolution:
		return "quarter_resolution";
	default:
		return "invalid";
	}
}
