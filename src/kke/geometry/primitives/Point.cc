#include <kke/geometry/primitives/Point.hh>

kke::Point2f::Point2f(float x, float y) : x(x), y(y) {
}

D2D1_POINT_2F kke::Point2f::asD2dPoint2f() const {
	return {x, y};
}
