#pragma once

#include "kke/appearance/resource/texture/TextureInterpolation.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class TextureSampling {
  public:
	static D2D1_INTERPOLATION_MODE convert(TextureInterpolation interpolation);
};
} // namespace kke
