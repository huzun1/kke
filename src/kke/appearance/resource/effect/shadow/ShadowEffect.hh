#pragma once

#include "kke/appearance/Color.hh"
#include "kke/appearance/resource/effect/shadow/ShadowMode.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct ShadowEffect {
	Point offset {0.0f, 0.0f};
	float blurStandardDeviation = 3.0f;
	Color color {0.0f, 0.0f, 0.0f, 1.0f};
	ShadowMode mode = ShadowMode::ShadowWithSource;
};
}	// namespace kke
