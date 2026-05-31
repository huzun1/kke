#pragma once

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class GeometryInverter {
  public:
	static Microsoft::WRL::ComPtr<ID2D1Geometry>
	create(D2dContext const& context, ID2D1Geometry* viewportGeometry, ID2D1Geometry* maskGeometry);
};
} // namespace kke
