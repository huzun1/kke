#pragma once

#include <cstdint>

namespace kke {
enum class GeometryHashTag : uint8_t {
	Triangle,
	Rect,
	RoundedRect,
	Ellipse,
	Polygon,
    Compose
};
};
