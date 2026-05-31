#include "ColorMatrixEffectRenderer.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Image> ColorMatrixEffectRenderer::render(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, ColorMatrixEffect const& effect
) const {
	ComPtr<ID2D1Effect> colorMatrixEffect;
	HRESULT result = context.getD2dContext()->getDeviceContext()->CreateEffect(
		CLSID_D2D1ColorMatrix,
		&colorMatrixEffect
	);
	if (FAILED(result) || !colorMatrixEffect) {
		return nullptr;
	}

	colorMatrixEffect->SetInput(0, sourceImage.Get());
	colorMatrixEffect->SetValue(
		D2D1_COLORMATRIX_PROP_COLOR_MATRIX,
		toD2dColorMatrix(effect.colorMatrix)
	);
	colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_ALPHA_MODE, mapAlphaMode(effect.alphaMode));
	colorMatrixEffect->SetValue(
		D2D1_COLORMATRIX_PROP_CLAMP_OUTPUT,
		static_cast<BOOL>(effect.clampOutput)
	);

	ComPtr<ID2D1Image> outputImage;
	colorMatrixEffect->GetOutput(&outputImage);
	return outputImage;
}

D2D1_MATRIX_5X4_F ColorMatrixEffectRenderer::toD2dColorMatrix(ColorMatrix const& colorMatrix) {
	std::array<float, 20> const& values = colorMatrix.values;
	return D2D1::Matrix5x4F(
		values[0],
		values[1],
		values[2],
		values[3],
		values[4],
		values[5],
		values[6],
		values[7],
		values[8],
		values[9],
		values[10],
		values[11],
		values[12],
		values[13],
		values[14],
		values[15],
		values[16],
		values[17],
		values[18],
		values[19]
	);
}

D2D1_COLORMATRIX_ALPHA_MODE ColorMatrixEffectRenderer::mapAlphaMode(ColorMatrixAlphaMode alphaMode
) {
	switch (alphaMode) {
	case ColorMatrixAlphaMode::Straight:
		return D2D1_COLORMATRIX_ALPHA_MODE_STRAIGHT;
	case ColorMatrixAlphaMode::Premultiplied:
	default:
		return D2D1_COLORMATRIX_ALPHA_MODE_PREMULTIPLIED;
	}
}
