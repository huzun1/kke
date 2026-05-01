#pragma once

#include <cstdint>

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"
#include "kke/engine/d2d/resource/geometry/GeometryProvider.hh"
#include "kke/engine/d2d/resource/geometry/invert/ViewportGeometryCache.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class InvertedGeometryProvider {
	KeyCacheStorage<ID2D1Geometry> geometries;
	ViewportGeometryCache viewportGeometryCache;
	D2D1_SIZE_F viewportSize = {};

public:
	void syncViewportSize(D2dContext const& context);

	Microsoft::WRL::ComPtr<ID2D1Geometry> getInvertedGeometry(
		D2dContext const& context, GeometryProvider& geometryProvider, Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Geometry> getInvertedGeometry(
		D2dContext const& context, GeometryProvider& geometryProvider, GeometryCompose const& compose);

private:
	bool isViewportResized(D2dContext const& context) const;

	uint64_t createKey(D2dContext const& context, Geometry const& geometry) const;

	uint64_t createKey(D2dContext const& context, GeometryCompose const& compose) const;

	Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context, GeometryProvider& geometryProvider, Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context, GeometryProvider& geometryProvider, GeometryCompose const& compose);

	Microsoft::WRL::ComPtr<ID2D1Geometry> createInvertedGeometry(
		D2dContext const& context, ID2D1Geometry* maskGeometry);
};
}	// namespace kke
