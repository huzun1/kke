#pragma once

#include <cstdint>

#include "kke/appearance/resource/brush/Brush.hh"

namespace kke {
class BrushHasher {
  public:
	static uint64_t hash(Brush const& brush);

	static uint64_t hashGradientStops(LinearGradientBrush const& brush);

  private:
	static uint64_t hash(SolidColorBrush const& brush);

	static uint64_t hash(LinearGradientBrush const& brush);

	static uint64_t hash(RasterSurfaceBrush const& brush);
};
} // namespace kke
