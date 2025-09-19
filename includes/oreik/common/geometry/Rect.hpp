#pragma once

#include <d2d1.h>

#include "oreik/common/Point.hpp"

namespace oreik {
struct Rect {
	float x1, y1, x2, y2;

	oreik::Point2f center() const;

	void offset(float x1, float y1, float x2, float y2);

	D2D1_RECT_F rectF() const;
};
};	// namespace oreik
