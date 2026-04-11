#pragma once

#define NOMINMAX
#include <d2d1.h>

namespace kke {
struct Point2f {
	float x, y;

	Point2f(float x, float y);

	D2D1_POINT_2F asD2dPoint2f() const;
};

#include "PointOperators.hh"

using Scale2f = Point2f;

};	// namespace kke
