#include <cstdint>
#include <oreik/common/geometry/Rect.hpp>

#include "oreik/common/Geometry.hpp"
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

uint64_t oreik::Rect::hash(bool positionDependent) const {
	Hasher hasher;
	hasher.combine(static_cast<uint32_t>(GeometryType::RECT));
	hasher.combine(positionDependent);
	if (positionDependent) {
		hasher.combine(x1);
		hasher.combine(y1);
		hasher.combine(x2);
		hasher.combine(y2);
	} else {
		hasher.combine(static_cast<uint32_t>(x2 - x1));
		hasher.combine(static_cast<uint32_t>(y2 - y1));
	}
	return hasher.get();
}
