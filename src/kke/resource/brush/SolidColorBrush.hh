#pragma once

#include "../common/Color.hh"
#include "Brush.hh"

namespace kke {
class SolidColorBrush : public Brush {
	kke::Color4f color;

public:
	SolidColorBrush(kke::Color4f color);

	void create(ID2D1DeviceContext* context, ID2D1Brush** output) const override;

	uint64_t hash() const override;
};
};	// namespace kke
