#pragma once

#include "kke/appearance/resource/effect/color_matrix/ColorMatrixEffect.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class ColorMatrixEffectRenderer {
  public:
	Microsoft::WRL::ComPtr<ID2D1Image> render(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		ColorMatrixEffect const& effect
	) const;

  private:
	static D2D1_MATRIX_5X4_F toD2dColorMatrix(ColorMatrix const& colorMatrix);

	static D2D1_COLORMATRIX_ALPHA_MODE mapAlphaMode(ColorMatrixAlphaMode alphaMode);
};
} // namespace kke
