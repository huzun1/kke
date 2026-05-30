#include "EffectRenderer.hh"

#include "kke/appearance/resource/effect/EffectIdentifier.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController) {
	ComPtr<ID2D1Image> sourceImage = renderPass.cycleTargetCommandList(context);
	if (!sourceImage) {
		return;
	}

	ComPtr<ID2D1Image> croppedSourceImage = cropSourceImage(context, sourceImage, clip);
	if (clip.has_value()) {
		drawImage(context, sourceImage, std::nullopt, viewLayerController);
	}

	drawImage(context, apply(context, croppedSourceImage, effect), clip, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController) {
	ComPtr<ID2D1Image> sourceImage = renderPass.cycleTargetCommandList(context);
	if (!sourceImage) {
		return;
	}

	ComPtr<ID2D1Image> croppedSourceImage = cropSourceImage(context, sourceImage, clip);
	if (clip.has_value()) {
		drawImage(context, sourceImage, std::nullopt, viewLayerController);
	}

	drawImage(context, apply(context, croppedSourceImage, effectCompose), clip, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController) {
	switch (EffectIdentifier::identify(effect)) {
	case EffectKind::Shadow: {
		auto result = shadowEffectRenderer.render(context, source, sourceAppearance, std::get<ShadowEffect>(effect), clip);
		drawImage(context, result.image, result.targetOffset, clip, viewLayerController);
		return;
	}
	case EffectKind::Blur: {
		auto result = blurEffectRenderer.render(context, source, sourceAppearance, std::get<BlurEffect>(effect), clip);
		drawImage(context, result.image, result.targetOffset, clip, viewLayerController);
		return;
	}
	default:
		break;
	}

	std::shared_ptr<D2dCanvas> sourceCanvas = sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return;
	}

	ComPtr<ID2D1Image> sourceImage = cropSourceImage(context, sourceCanvas->getCommandList(), clip);
	drawImage(context, apply(context, sourceImage, effect), clip, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController) {
	std::shared_ptr<D2dCanvas> sourceCanvas = sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return;
	}

	ComPtr<ID2D1Image> sourceImage = cropSourceImage(context, sourceCanvas->getCommandList(), clip);
	drawImage(context, apply(context, sourceImage, effectCompose), clip, viewLayerController);
}

ComPtr<ID2D1Image> EffectRenderer::apply(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	Effect const& effect) {
	switch (EffectIdentifier::identify(effect)) {
	case EffectKind::Shadow:
		return shadowEffectRenderer.render(context, sourceImage, std::get<ShadowEffect>(effect));
	case EffectKind::Blur:
		return blurEffectRenderer.render(context, sourceImage, std::get<BlurEffect>(effect));
	case EffectKind::DirectionalBlur:
		return directionalBlurEffectRenderer.render(context, sourceImage, std::get<DirectionalBlurEffect>(effect));
	case EffectKind::ColorMatrix:
		return colorMatrixEffectRenderer.render(context, sourceImage, std::get<ColorMatrixEffect>(effect));
	default:
		return nullptr;
	}
}

ComPtr<ID2D1Image> EffectRenderer::apply(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	EffectCompose const& effectCompose) {
	ComPtr<ID2D1Image> currentImage = sourceImage;

	for (Effect const& effect : effectCompose.getCompose()) {
		currentImage = apply(context, currentImage, effect);
		if (!currentImage) {
			return nullptr;
		}
	}

	return currentImage;
}

ComPtr<ID2D1Image> EffectRenderer::cropSourceImage(
	D2dEngineContext& context,
	ComPtr<ID2D1Image> sourceImage,
	std::optional<EffectClipSource> const& clip) {
	if (!clip.has_value()) {
		return sourceImage;
	}

	Geometry const* geometry = std::get_if<Geometry>(&clip.value());
	if (!geometry) {
		return sourceImage;
	}

	Rect const* rect = std::get_if<Rect>(geometry);
	if (!rect) {
		return sourceImage;
	}

	ComPtr<ID2D1Effect> cropEffect;
	HRESULT cropResult = context.getD2dContext()->getDeviceContext()->CreateEffect(CLSID_D2D1Crop, &cropEffect);
	if (FAILED(cropResult) || !cropEffect) {
		return sourceImage;
	}

	cropEffect->SetInput(0, sourceImage.Get());
	cropEffect->SetValue(D2D1_CROP_PROP_RECT, D2D1::Vector4F(rect->min.x, rect->min.y, rect->max.x, rect->max.y));
	cropEffect->SetValue(D2D1_CROP_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);

	ComPtr<ID2D1Image> croppedImage;
	cropEffect->GetOutput(&croppedImage);
	return croppedImage ? croppedImage : sourceImage;
}

void EffectRenderer::drawImage(
	D2dEngineContext const& context,
	ComPtr<ID2D1Image> image,
	std::optional<EffectClipSource> const& clip,
	ViewLayerController& viewLayerController) {
	if (!image) {
		return;
	}

	if (!clip.has_value()) {
		context.getD2dContext()->getDeviceContext()->DrawImage(image.Get());
		return;
	}

	viewLayerController.pushLayer(context, clip.value(), LayerMode::Normal);
	context.getD2dContext()->getDeviceContext()->DrawImage(image.Get());
	viewLayerController.popLayer(context);
}

void EffectRenderer::drawImage(
	D2dEngineContext const& context,
	ComPtr<ID2D1Image> image,
	Point const& targetOffset,
	std::optional<EffectClipSource> const& clip,
	ViewLayerController& viewLayerController) {
	if (!image) {
		return;
	}

	if (!clip.has_value()) {
		context.getD2dContext()->getDeviceContext()->DrawImage(image.Get(), {targetOffset.x, targetOffset.y});
		return;
	}

	viewLayerController.pushLayer(context, clip.value(), LayerMode::Normal);
	context.getD2dContext()->getDeviceContext()->DrawImage(image.Get(), {targetOffset.x, targetOffset.y});
	viewLayerController.popLayer(context);
}
