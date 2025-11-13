#include <kke/common/geometry/RoundedRect.hpp>

#include "kke/internal/Hasher.hpp"

kke::RoundedRect::RoundedRect(float x1, float y1, float x2, float y2, float rounding)
	: Rect({x1, y1, x2, y2}),
	  rounding(rounding) {
}

kke::RoundedRect::RoundedRect(Rect const& rect, float rounding)
	: Rect(rect),
	  rounding(rounding) {
}

uint64_t kke::RoundedRect::hash(bool positionDependent) const {
	Hasher hasher;
	hasher.combine(Rect::hash(positionDependent));
	hasher.combine(rounding);
	return hasher.get();
}

D2D1_ROUNDED_RECT kke::RoundedRect::roundedRect() const {
	return D2D1::RoundedRect(rectF(), rounding, rounding);
}
