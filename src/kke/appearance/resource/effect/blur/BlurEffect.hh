#pragma once

#include "kke/appearance/resource/effect/blur/BlurAppearance.hh"

namespace kke {
struct BlurEffect {
	BlurAppearance appearance {};

	BlurEffect() = default;

	BlurEffect(float radius)
		: appearance({radius, BlurBorderMode::SOFT, BlurOptimization::BALANCED, BlurMode::Normal}) {
	}

	BlurEffect(BlurAppearance const& appearance)
		: appearance(appearance) {
	}
};
}	// namespace kke
