#include <kke/geometry/shapes/Rect.hh>

using namespace kke;

Rect::Rect() : min(0, 0), max(0, 0) {
}

Rect::Rect(kke::Point const& min, kke::Point const& max) : min(min), max(max) {
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

kke::Point Rect::center() const {
	return (min + max) / 2;
}

Rect Rect::shrink(float amount) const {
	return Rect(min + Point(amount, amount), max - Point(amount, amount));
}

Rect Rect::expand(float amount) const {
	return Rect(min - Point(amount, amount), max + Point(amount, amount));
}

