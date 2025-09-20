#pragma once

#include <d2d1_1.h>
#include <winnt.h>

#include <oreik/effect/Effect.hpp>
#include <oreik/internal/Macro.hpp>

namespace oreik {
enum class BlurOptimization {
	SPEED,	   // Applies internal optimizations such as pre-scaling at relatively small radii. Uses linear filtering.
	BALANCED,  // Uses the same optimization thresholds as Speed mode, but uses trilinear filtering.
	QUALITY	   // Only uses internal optimizations with large blur radii, where approximations are less likely to be visible. Uses trilinear filtering.
};

enum class BlurBorderMode {
	SOFT,  // If the crop rectangle falls on fractional pixel coordinates, the effect applies antialiasing which results in a soft edge.
	HARD   // If the crop rectangle falls on fractional pixel coordinates, the effect clamps which results in a hard edge.
};

class BlurEffect : public Effect {
	float deviation = 0.0f;
	BlurOptimization optimization = BlurOptimization::BALANCED;
	BlurBorderMode borderMode;

public:
	GUID effectGuid() const override;

	DEFINE_GETTER_SETTER(float, Deviation, deviation);
	DEFINE_GETTER_SETTER(BlurOptimization, Optimization, optimization);
	DEFINE_GETTER_SETTER(BlurBorderMode, BorderMode, borderMode);

	void setProperties() const override;
};
};	// namespace oreik
