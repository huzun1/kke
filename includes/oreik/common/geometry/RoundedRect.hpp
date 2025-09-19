#pragma once

#include "oreik/common/geometry/Rect.hpp"

namespace oreik {
struct RoundedRect : public Rect {
	float rounding;

	RoundedRect(Rect const& rect, float rounding);
};
};	// namespace oreik
