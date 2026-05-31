#pragma once

#include "kke/geometry/shapes/Rect.hh"

namespace kke {
struct RoundedRect : public Rect {
	float rounding;

	~RoundedRect() = default;

	RoundedRect();

	RoundedRect(Rect const& rect, float rounding);
};
}; // namespace kke
