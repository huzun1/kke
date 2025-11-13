#pragma once

#include <d2d1.h>

#include <cstdint>

namespace kke {
struct Point2f {
	float x, y;

	D2D1_POINT_2F point2f() const;

	uint64_t hash() const;
};
};	// namespace kke
