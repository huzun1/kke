#pragma once

#include "../common/Color.hpp"
#include "Brush.hpp"

namespace oreik {
class SolidColorBrush : public Brush {
	oreik::Color4f color;

public:
	SolidColorBrush(oreik::Color4f color);

	void create(ID2D1DeviceContext* context, ID2D1Brush** output) const override;

	uint64_t hash() const override;
};
};	// namespace oreik
