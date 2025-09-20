#include <cstdint>
#include <oreik/common/geometry/Ellipse.hpp>

#include "oreik/common/Geometry.hpp"
#include "oreik/internal/Hasher.hpp"

oreik::Ellipse::Ellipse(float x, float y, float radius)
	: point(oreik::Point2f{x, y}), radius(radius) {
}

D2D1_ELLIPSE oreik::Ellipse::ellipse() const {
	return {point.point2f(), radius, radius};
}

uint64_t oreik::Ellipse::hash(bool positionDependent) const {
	Hasher hasher;
	hasher.combine(static_cast<uint32_t>(GeometryType::ELLIPSE));
	hasher.combine(positionDependent);
	if (positionDependent) {
		hasher.combine(point.x);
		hasher.combine(point.y);
	}
	hasher.combine(radius);
	return hasher.get();
}
