#include "RoundedRect.hh"

using namespace kke;

RoundedRect::RoundedRect()
	: Rect({0.0f, 0.0f}, {0.0f, 0.0f}),
	  rounding(0.0f) {
}

RoundedRect::RoundedRect(Rect const& rect, float rounding)
	: Rect(rect),
	  rounding(rounding) {
}
