#pragma once

namespace kke {
struct Point {
	float x, y;

	Point(float x, float y);
};

#include "PointOperators.hh"

using Scale = Point;

};	// namespace kke
