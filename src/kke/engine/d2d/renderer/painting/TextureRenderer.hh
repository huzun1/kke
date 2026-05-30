#pragma once

#include <memory>

#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke {
class TextureRenderer {
public:
	void draw(
		D2dEngineContext const& context,
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		TextureDrawAppearance const& appearance);

private:
	static D2D1_RECT_F toD2dRect(Rect const& rect);

	static D2D1_BITMAP_INTERPOLATION_MODE toD2dInterpolation(TextureInterpolation interpolation);
};
}	// namespace kke
