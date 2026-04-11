#include <kke/geometry/shapes/polygon/Rect.hh>

using namespace kke;

Rect::Rect() : min(0, 0), max(0, 0) {
}

Rect::Rect(kke::Point2f const& min, kke::Point2f const& max) : min(min), max(max) {
}

Boundary Rect::getBounding() const {
	return {min.x, min.y, max.x, max.y};
}

float Rect::width() const {
	return max.x - min.x;
}

float Rect::height() const {
	return max.y - min.y;
}

kke::Point2f Rect::center() const {
	return (min + max) / 2;
}

Rect Rect::shrink(float amount) const {
	return Rect(min + Point2f(amount, amount), max - Point2f(amount, amount));
}

Rect Rect::expand(float amount) const {
	return Rect(min - Point2f(amount, amount), max + Point2f(amount, amount));
}

D2D1_RECT_F Rect::asD2dRectF() const {
	return D2D1_RECT_F{min.x, min.y, max.x, max.y};
}
