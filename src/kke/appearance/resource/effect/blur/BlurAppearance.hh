#pragma once

#include "BlurBorderMode.hh"
#include "BlurMode.hh"
#include "BlurOptimization.hh"

namespace kke {
struct BlurAppearance {
	float radius = 3.0f;
	BlurBorderMode borderMode = BlurBorderMode::SOFT;
	BlurOptimization optimization = BlurOptimization::BALANCED;
	BlurMode mode = BlurMode::Normal;
};
}	// namespace kke
