#pragma once

#include "../common/Color.hpp"
#include "Brush.hpp"

namespace oreik {
class SolidColorBrush : public Brush {
	oreik::Color4f color;

public:
	SolidColorBrush(oreik::Color4f color);

	ID2D1Brush* create(ID2D1DeviceContext* context) const;

	uint64_t hash() const;
};
};	// namespace oreik
