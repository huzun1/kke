#pragma once

#include <memory>

#include "kke/appearance/resource/RasterSurface.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke {
class RasterSurfaceBrush {
	std::shared_ptr<RasterSurface> surface;
	Rect destination;
	float opacity;

  public:
	RasterSurfaceBrush(
		std::shared_ptr<RasterSurface> surface, Rect destination, float opacity = 1.0f
	);

	std::shared_ptr<RasterSurface> const& getSurface() const;

	Rect const& getDestination() const;

	float getOpacity() const;
};
} // namespace kke
