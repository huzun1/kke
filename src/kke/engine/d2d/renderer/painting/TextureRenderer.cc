#include "TextureRenderer.hh"

#include <memory>

#include "kke/engine/d2d/resource/texture/D2dTexture.hh"

using namespace kke;

void TextureRenderer::draw(
	D2dEngineContext const& context,
	std::shared_ptr<Texture> texture,
	Rect const& destRect,
	TextureDrawAppearance const& appearance
) {
	std::shared_ptr<D2dTexture> d2dTexture = std::dynamic_pointer_cast<D2dTexture>(texture);
	if (!d2dTexture) {
		return;
	}

	D2D1_RECT_F d2dDestRect = toD2dRect(destRect);
	std::optional<D2D1_RECT_F> d2dSourceRect = std::nullopt;
	if (appearance.srcRect) {
		d2dSourceRect = toD2dRect(*appearance.srcRect);
	}

	context.getD2dContext()->getDeviceContext()->DrawBitmap(
		d2dTexture->getBitmap().Get(),
		d2dDestRect,
		appearance.opacity,
		toD2dInterpolation(appearance.interpolation),
		d2dSourceRect ? &*d2dSourceRect : nullptr
	);
}

D2D1_RECT_F TextureRenderer::toD2dRect(Rect const& rect) {
	return {rect.min.x, rect.min.y, rect.max.x, rect.max.y};
}

D2D1_INTERPOLATION_MODE
TextureRenderer::toD2dInterpolation(TextureInterpolation interpolation) {
	switch (interpolation) {
	case TextureInterpolation::Nearest:
		return D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
	case TextureInterpolation::Linear:
		return D2D1_INTERPOLATION_MODE_LINEAR;
	case TextureInterpolation::HighQualityCubic:
		return D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
	default:
		return D2D1_INTERPOLATION_MODE_LINEAR;
	}
}
