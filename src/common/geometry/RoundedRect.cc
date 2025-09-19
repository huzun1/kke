#include <oreik/common/geometry/RoundedRect.hpp>

oreik::RoundedRect::RoundedRect(Rect const& rect, float rounding)
	: Rect(rect),
	  rounding(rounding) {
}
