#pragma once

#include <memory>

namespace kke {
class RasterSurface;

struct EffectCaptureOptions {
	float rasterScale = 1.0f;
	std::shared_ptr<RasterSurface> reusableSurface;
};
} // namespace kke
