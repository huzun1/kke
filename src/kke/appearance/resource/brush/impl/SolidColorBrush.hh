#pragma once

#include "kke/appearance/Color.hh"

namespace kke {
class SolidColorBrush {
	Color color;

public:
	SolidColorBrush(Color color);

	Color const& getColor() const;
};
};	// namespace kke
