#include <kke/common/geometry/RoundedRect.hh>

#include "kke/internal/Hasher.hh"

kke::RoundedRect::RoundedRect(float x1, float y1, float x2, float y2, float rounding)
	: Rect({x1, y1, x2, y2}),
	  rounding(rounding) {
}

kke::RoundedRect::RoundedRect(Rect const& rect, float rounding)
	: Rect(rect),
	  rounding(rounding) {
}

void kke::RoundedRect::create(ID2D1Factory* factory, ID2D1Geometry** output) const {
	ID2D1RoundedRectangleGeometry* roundedRectangleGeometry;
	factory->CreateRoundedRectangleGeometry(roundedRect(), &roundedRectangleGeometry);
	*output = roundedRectangleGeometry;
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
