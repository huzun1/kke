#pragma once

#include <d2d1.h>

#include "kke/geometry/shapes/polygon/Rect.hh"

namespace kke {
struct RoundedRect : public Rect {
	float rounding;
	
	RoundedRect();

	RoundedRect(Rect const& rect, float rounding);
};
};	// namespace kke
