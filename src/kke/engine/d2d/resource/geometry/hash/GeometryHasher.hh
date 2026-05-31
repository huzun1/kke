#pragma once

#include <cstdint>

#include "GeometryHashMode.hh"
#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"

// NOTE: I don't know that should I place those logic in Geometry class or not.
// If the hasing logic is required for other backends, it should be placed in Geometry class.
// But currently not.

namespace kke {
class GeometryHasher {
  public:
	static uint64_t hash(Geometry const& geometry, GeometryHashMode mode);

	static uint64_t hash(GeometryCompose const& compose, GeometryHashMode mode);

  private:
	static uint64_t hash(Triangle const& triangle, GeometryHashMode mode);

	static uint64_t hash(Rect const& rect, GeometryHashMode mode);

	static uint64_t hash(RoundedRect const& roundedRect, GeometryHashMode mode);

	static uint64_t hash(Ellipse const& ellipse, GeometryHashMode mode);

	static uint64_t hash(Polygon const& polygon, GeometryHashMode mode);
};
}; // namespace kke
