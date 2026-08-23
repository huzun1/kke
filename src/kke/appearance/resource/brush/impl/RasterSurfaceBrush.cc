#include "RasterSurfaceBrush.hh"

#include <utility>

using namespace kke;

RasterSurfaceBrush::RasterSurfaceBrush(
	std::shared_ptr<RasterSurface> surface, Rect destination, float opacity
)
	: surface(std::move(surface)), destination(destination), opacity(opacity) {
}

std::shared_ptr<RasterSurface> const& RasterSurfaceBrush::getSurface() const {
	return surface;
}

Rect const& RasterSurfaceBrush::getDestination() const {
	return destination;
}

float RasterSurfaceBrush::getOpacity() const {
	return opacity;
}
