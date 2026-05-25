#pragma once

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class ViewportGeometryCache {
	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry;
	D2D1_SIZE_F viewportGeometrySize = {};

public:
	void syncViewportSize(D2dContext const& context);

	Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> get(D2dContext const& context);

private:
	bool isViewportResized(D2D1_SIZE_F viewportSize) const;
};
}	// namespace kke
