#include "EffectClipCropper.hh"

#include "kke/engine/d2d/renderer/effect/EffectClipBoundsResolver.hh"
#include "kke/engine/d2d/renderer/effect/EffectPaddingEstimator.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Image> EffectClipCropper::crop(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	Effect const& effect,
	EffectClipSource const& clip
) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_RECT_F cropBounds = expandRect(
		EffectClipBoundsResolver::resolve(clip),
		EffectPaddingEstimator::estimate(effect)
	);

	ComPtr<ID2D1Effect> cropEffect;
	cropEffect =
		context.getResourceProviders()->getEffectPool()->acquire(deviceContext, CLSID_D2D1Crop);
	if (!cropEffect) {
		return sourceImage;
	}

	cropEffect->SetInput(0, sourceImage.Get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, cropBounds);

	ComPtr<ID2D1Image> croppedImage;
	cropEffect->GetOutput(&croppedImage);
	return croppedImage ? croppedImage : sourceImage;
}

ComPtr<ID2D1Image> EffectClipCropper::crop(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	EffectCompose const& effectCompose,
	EffectClipSource const& clip
) const {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_RECT_F cropBounds = expandRect(
		EffectClipBoundsResolver::resolve(clip),
		EffectPaddingEstimator::estimate(effectCompose)
	);

	ComPtr<ID2D1Effect> cropEffect;
	cropEffect =
		context.getResourceProviders()->getEffectPool()->acquire(deviceContext, CLSID_D2D1Crop);
	if (!cropEffect) {
		return sourceImage;
	}

	cropEffect->SetInput(0, sourceImage.Get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, cropBounds);

	ComPtr<ID2D1Image> croppedImage;
	cropEffect->GetOutput(&croppedImage);
	return croppedImage ? croppedImage : sourceImage;
}

D2D1_RECT_F EffectClipCropper::expandRect(D2D1_RECT_F const& rect, float amount) {
	return D2D1::RectF(
		rect.left - amount,
		rect.top - amount,
		rect.right + amount,
		rect.bottom + amount
	);
}
