#include <oreik/common/geometry/RoundedRect.hpp>

#include "oreik/internal/Hasher.hpp"

oreik::RoundedRect::RoundedRect(float x1, float y1, float x2, float y2, float rounding)
	: Rect({x1, y1, x2, y2}),
	  rounding(rounding) {
}

oreik::RoundedRect::RoundedRect(Rect const& rect, float rounding)
	: Rect(rect),
	  rounding(rounding) {
}

uint64_t oreik::RoundedRect::hash() const {
	Hasher hasher;
	hasher.combine(x1);
	hasher.combine(y1);
	hasher.combine(x2);
	hasher.combine(y2);
	hasher.combine(rounding);
	return hasher.get();
}

D2D1_ROUNDED_RECT oreik::RoundedRect::roundedRect() const {
	return D2D1::RoundedRect(rectF(), rounding, rounding);
}
