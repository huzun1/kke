#pragma once

#include <d2d1.h>

#include <cstdint>

#include "kke/common/geometry/Rect.hh"

namespace kke {
struct RoundedRect : public Rect {
	float rounding;

	RoundedRect(float x1, float y1, float x2, float y2, float rounding);

	RoundedRect(Rect const& rect, float rounding);

	void create(ID2D1Factory* factory, ID2D1Geometry** output) const override;

	uint64_t hash(bool positionDependent) const override;

	D2D1_ROUNDED_RECT roundedRect() const;
};
};	// namespace kke
