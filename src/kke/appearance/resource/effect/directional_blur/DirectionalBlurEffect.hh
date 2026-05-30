#pragma once

#include "kke/appearance/resource/effect/blur/BlurBorderMode.hh"
#include "kke/appearance/resource/effect/blur/BlurOptimization.hh"

namespace kke {
struct DirectionalBlurEffect {
	float standardDeviation = 3.0f;
	float angle = 0.0f;
	BlurBorderMode borderMode = BlurBorderMode::SOFT;
	BlurOptimization optimization = BlurOptimization::BALANCED;
};
}	// namespace kke
