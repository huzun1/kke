#pragma once

#include "kke/appearance/Scale.hh"
#include "kke/appearance/Text.hh"

namespace kke {
class Measurer {
public:
	Scale getViewportSize();

	Scale measureTextSize(
		Text const& text);
};
};	// namespace kke
