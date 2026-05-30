#pragma once

#include <vector>

#include "kke/geometry/Geometry.hh"

namespace kke {
class GeometryCompose {
	std::vector<Geometry> geometries;

public:
	void add(Geometry const& geometry);

    std::vector<Geometry> const& getGeometries() const;
};
};	// namespace kke
