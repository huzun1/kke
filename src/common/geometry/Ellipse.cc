#include "kke/common/geometry/Ellipse.hh"

#include <cstdint>

#include "kke/common/Geometry.hh"
#include "kke/internal/Hasher.hh"

kke::Ellipse::Ellipse(float x, float y, float radius)
	: point(kke::Point2f{x, y}), radius(radius) {
}

void kke::Ellipse::create(ID2D1Factory* factory, ID2D1Geometry** output) const {
	ID2D1EllipseGeometry* ellipseGeometry;
	factory->CreateEllipseGeometry(ellipse(), &ellipseGeometry);
	*output = ellipseGeometry;
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
