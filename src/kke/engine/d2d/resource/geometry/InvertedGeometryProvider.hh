#pragma once

#include <cstdint>

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"
#include "kke/engine/d2d/resource/geometry/GeometryProvider.hh"
#include "kke/engine/d2d/resource/geometry/ViewportGeometryCache.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class InvertedGeometryProvider {
	KeyCacheStorage<ID2D1Geometry> geometries;
	ViewportGeometryCache viewportGeometryCache;
	D2D1_SIZE_F viewportSize = {};

  public:
	void syncViewportSize(D2dContext const& context, D2D1_SIZE_F viewportSize);

	Microsoft::WRL::ComPtr<ID2D1Geometry>
	get(D2dContext const& context, GeometryProvider& geometryProvider, Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Geometry>
	get(D2dContext const& context,
		GeometryProvider& geometryProvider,
		GeometryCompose const& compose);

  private:
	uint64_t createInvertedGeometryKey(D2dContext const& context, Geometry const& geometry);

	uint64_t createInvertedGeometryKey(D2dContext const& context, GeometryCompose const& compose);

	bool isViewportResized(D2D1_SIZE_F viewportSize) const;
};
} // namespace kke
