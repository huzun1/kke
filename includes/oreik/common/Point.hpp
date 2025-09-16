#pragma once

#include <d2d1.h>

namespace oreik {
struct Point2f {
	float x, y;

	D2D1_POINT_2F point2f() const;
};
};	// namespace oreik
