#pragma once

#include <cstdint>

#include "kke/appearance/Color.hh"

namespace kke {
class ColorQuantizer {
  public:
	static Color quantize(Color const& color);

	static uint8_t quantizeComponent(float component);

	static float toFloat(uint8_t component);
};
} // namespace kke
