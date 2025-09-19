#include <oreik/common/geometry/Ellipse.hpp>

oreik::Ellipse::Ellipse(float x, float y, float radius)
	: point(oreik::Point2f{x, y}), radius(radius) {
}

D2D1_ELLIPSE oreik::Ellipse::ellipse() const {
	return {point.point2f(), radius, radius};
}
