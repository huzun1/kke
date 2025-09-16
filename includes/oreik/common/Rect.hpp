#pragma once

#include <d2d1.h>

namespace oreik {
struct Rect {
	float x1, y1, x2, y2;

	void offset(float x1, float y1, float x2, float y2);

	D2D1_RECT_F rectF() const;
};
};	// namespace oreik
