#include <oreik/common/geometry/Rect.hpp>

#include "oreik/internal/Hasher.hpp"

oreik::Rect::Rect(float x1, float y1, float x2, float y2)
	: x1(x1),
	  y1(y1),
	  x2(x2),
	  y2(y2) {
}

oreik::Point2f oreik::Rect::center() const {
	return {x1 + (x2 - x1) * 0.5f, y1 + (y2 - y1) * 0.5f};
}

void oreik::Rect::offset(float x1, float y1, float x2, float y2) {
	this->x1 += x1;
	this->y1 += y1;
	this->x2 += x2;
	this->y2 += y2;
}

D2D1_RECT_F oreik::Rect::rectF() const {
	return {x1, y1, x2, y2};
}

uint64_t oreik::Rect::hash() const {
	Hasher hasher;
	hasher.combine(x1);
	hasher.combine(y1);
	hasher.combine(x2);
	hasher.combine(y2);
	return hasher.get();
}
