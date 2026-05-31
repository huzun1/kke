#pragma once

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/resource/geometry/GeometryProvider.hh"

namespace kke {
class InvertedGeometryFactory {
  public:
	static Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context,
		GeometryProvider& geometryProvider,
		Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
		Geometry const& geometry
	);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context,
		GeometryProvider& geometryProvider,
		Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
		GeometryCompose const& compose
	);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context,
		Microsoft::WRL::ComPtr<ID2D1RectangleGeometry> viewportGeometry,
		ID2D1Geometry* maskGeometry
	);
};
}; // namespace kke
