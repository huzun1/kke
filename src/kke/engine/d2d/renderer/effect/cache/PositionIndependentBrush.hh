#pragma once

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
class PositionIndependentBrush {
public:
	static Brush normalize(Brush const& brush, Point const& origin);

	static uint64_t hash(Brush const& brush, Point const& origin);
};
}	// namespace kke
