#pragma once

#include <cstdint>

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class GeometryProvider {
	KeyCacheStorage<ID2D1Geometry> storage;

  public:
	GeometryProvider(uint32_t limit = UINT32_MAX);

	Microsoft::WRL::ComPtr<ID2D1Geometry> get(D2dContext const& context, Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Geometry>
	get(D2dContext const& context, GeometryCompose const& compose);
};
} // namespace kke
