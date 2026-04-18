#pragma once

namespace kke {
enum class BlurOptimization {
	SPEED,	   // Applies internal optimizations such as pre-scaling at relatively small radii. Uses linear filtering.
	BALANCED,  // Uses the same optimization thresholds as Speed mode, but uses trilinear filtering.
	QUALITY	   // Only uses internal optimizations with large blur radii, where approximations are less likely to be visible. Uses trilinear filtering.
};
};
