#pragma once

namespace kke::benchmark {
enum class ShadowScenario {
	NoShadow,
	Cached,
	CacheMiss,
	CachedWithoutClip,
	Disabled,
	CachedBitmap,
	HalfResolution,
	QuarterResolution
};
} // namespace kke::benchmark
