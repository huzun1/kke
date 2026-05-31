#pragma once

#include <vector>

#include "kke/geometry/Geometry.hh"

namespace kke {
/**
 * @brief Stores multiple geometry sources as a single composite source.
 */
class GeometryCompose {
	std::vector<Geometry> geometries;

  public:
	/**
	 * @brief Appends a geometry to the composition in draw order.
	 */
	void add(Geometry const& geometry);

	/**
	 * @brief Returns the geometries currently stored in the composition.
	 */
	std::vector<Geometry> const& getGeometries() const;
};
} // namespace kke
