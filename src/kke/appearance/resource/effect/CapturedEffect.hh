#pragma once

#include <memory>

#include "kke/appearance/resource/RasterSurface.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke {
struct CapturedEffect {
	std::shared_ptr<RasterSurface> surface;
	Rect bounds;
};
} // namespace kke
