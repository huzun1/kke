#pragma once

#include <d2d1.h>

#include <cstdint>

#include "oreik/common/geometry/Rect.hpp"

namespace oreik {
struct RoundedRect : public Rect {
	float rounding;

	RoundedRect(float x1, float y1, float x2, float y2, float rounding);

	RoundedRect(Rect const& rect, float rounding);

	D2D1_ROUNDED_RECT roundedRect() const;

	uint64_t hash(bool positionDependent) const override;
};
};	// namespace oreik
