#pragma once

namespace kke {
struct Point2f {
	float x, y;

	Point2f(float x, float y);
};

#include "PointOperators.hh"

using Scale2f = Point2f;

};	// namespace kke
