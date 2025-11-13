#include <cstdint>
#include <kke/common/geometry/Ellipse.hpp>

#include "kke/common/Geometry.hpp"
#include "kke/internal/Hasher.hpp"

kke::Ellipse::Ellipse(float x, float y, float radius)
	: point(kke::Point2f{x, y}), radius(radius) {
}

D2D1_ELLIPSE kke::Ellipse::ellipse() const {
	return {point.point2f(), radius, radius};
}

kke::Rect kke::Ellipse::dimension() const {
	return {point.x - radius, point.y - radius, point.x + radius, point.y + radius};
}

uint64_t kke::Ellipse::hash(bool positionDependent) const {
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
