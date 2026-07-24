#include "EffectRenderer.hh"

#include "kke/appearance/resource/effect/EffectIdentifier.hh"
#include "kke/engine/d2d/renderer/effect/EffectClipTransformer.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	float opacity,
	ViewLayerController& viewLayerController
) {
	renderViewportAlignedEffect(context, renderPass, effect, clip, opacity, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	renderViewportAlignedEffect(context, renderPass, effectCompose, clip, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	switch (EffectIdentifier::identify(effect)) {
	case EffectKind::Shadow: {
		auto result =
			shadowEffectRenderer
				.render(context, source, sourceAppearance, std::get<ShadowEffect>(effect), clip);
		drawImage(context, result.image, result.targetOffset, clip, viewLayerController);
		return;
	}
	case EffectKind::Blur: {
		auto result =
			blurEffectRenderer
				.render(context, source, sourceAppearance, std::get<BlurEffect>(effect), clip);
		drawImage(context, result.image, result.targetOffset, clip, viewLayerController);
		return;
	}
	default:
		break;
	}

	std::shared_ptr<D2dCanvas> sourceCanvas =
		sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return;
	}

	drawImage(
		context,
		apply(context, sourceCanvas->getCommandList(), effect),
		clip,
		viewLayerController
	);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	std::shared_ptr<D2dCanvas> sourceCanvas =
		sourceRenderer.render(context, source, sourceAppearance);
	if (!sourceCanvas) {
		return;
	}

	drawImage(
		context,
		apply(context, sourceCanvas->getCommandList(), effectCompose),
		clip,
		viewLayerController
	);
}

ComPtr<ID2D1Image> EffectRenderer::apply(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, Effect const& effect
) {
	switch (EffectIdentifier::identify(effect)) {
	case EffectKind::Shadow:
		return shadowEffectRenderer.render(context, sourceImage, std::get<ShadowEffect>(effect));
	case EffectKind::Blur:
		return blurEffectRenderer.render(context, sourceImage, std::get<BlurEffect>(effect));
	case EffectKind::DirectionalBlur:
		return directionalBlurEffectRenderer
			.render(context, sourceImage, std::get<DirectionalBlurEffect>(effect));
	case EffectKind::ColorMatrix:
		return colorMatrixEffectRenderer
			.render(context, sourceImage, std::get<ColorMatrixEffect>(effect));
	default:
		return nullptr;
	}
}

ComPtr<ID2D1Image> EffectRenderer::apply(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, EffectCompose const& effectCompose
) {
	ComPtr<ID2D1Image> currentImage = sourceImage;

	for (Effect const& effect : effectCompose.getCompose()) {
		currentImage = apply(context, currentImage, effect);
		if (!currentImage) {
			return nullptr;
		}
	}

	return currentImage;
}

ComPtr<ID2D1Image> EffectRenderer::applyOpacity(
	D2dEngineContext& context, ComPtr<ID2D1Image> sourceImage, float opacity
) {
	ColorMatrixEffect opacityEffect;
	opacityEffect.colorMatrix.values = {
		opacity, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, opacity, 0.0f, 0.0f,	  0.0f,
		0.0f,	 0.0f, opacity, 0.0f, 0.0f, 0.0f, 0.0f,	   0.0f, opacity, 0.0f,
	};
	return colorMatrixEffectRenderer.render(context, sourceImage, opacityEffect);
}

void EffectRenderer::renderViewportAlignedEffect(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	float opacity,
	ViewLayerController& viewLayerController
) {
	if (opacity <= 0.0f) {
		return;
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_MATRIX_3X2_F activeTransform;
	deviceContext->GetTransform(&activeTransform);

	std::optional<EffectClipSource> viewportClip;
	if (clip.has_value()) {
		viewportClip = EffectClipTransformer::transform(*clip, activeTransform);
	}

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	ComPtr<ID2D1Image> sourceImage =
		renderPass.cycleTargetSnapshot(context, SnapshotOpacityMode::FlattenToOpaqueBlack);
	if (sourceImage) {
		if (viewportClip.has_value()) {
			sourceImage = clipCropper.crop(context, sourceImage, effect, viewportClip.value());
		}
		ComPtr<ID2D1Image> effectImage = apply(context, sourceImage, effect);
		bool useSourceOver = opacity < 1.0f;
		if (useSourceOver) {
			effectImage = applyOpacity(context, effectImage, opacity);
		}
		drawImage(context, effectImage, viewportClip, viewLayerController, useSourceOver);
	}
	deviceContext->SetTransform(activeTransform);
}

void EffectRenderer::renderViewportAlignedEffect(
	D2dEngineContext& context,
	RenderPass& renderPass,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_MATRIX_3X2_F activeTransform;
	deviceContext->GetTransform(&activeTransform);

	std::optional<EffectClipSource> viewportClip;
	if (clip.has_value()) {
		viewportClip = EffectClipTransformer::transform(*clip, activeTransform);
	}

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	ComPtr<ID2D1Image> sourceImage =
		renderPass.cycleTargetSnapshot(context, SnapshotOpacityMode::FlattenToOpaqueBlack);
	if (sourceImage) {
		if (viewportClip.has_value()) {
			sourceImage =
				clipCropper.crop(context, sourceImage, effectCompose, viewportClip.value());
		}
		drawImage(
			context,
			apply(context, sourceImage, effectCompose),
			viewportClip,
			viewLayerController
		);
	}
	deviceContext->SetTransform(activeTransform);
}

void EffectRenderer::drawImage(
	D2dEngineContext const& context,
	ComPtr<ID2D1Image> image,
	std::optional<EffectClipSource> const& clip,
	ViewLayerController& viewLayerController,
	bool useSourceOver
) {
	if (!image) {
		return;
	}

	if (!clip.has_value()) {
		context.getD2dContext()->getDeviceContext()->DrawImage(image.Get());
		return;
	}

	viewLayerController.pushLayer(context, clip.value(), LayerMode::Normal);
	context.getD2dContext()->getDeviceContext()->DrawImage(
		image.Get(),
		D2D1_INTERPOLATION_MODE_LINEAR,
		useSourceOver ? D2D1_COMPOSITE_MODE_SOURCE_OVER : D2D1_COMPOSITE_MODE_SOURCE_COPY
	);
	viewLayerController.popLayer(context);
}

void EffectRenderer::drawImage(
	D2dEngineContext const& context,
	ComPtr<ID2D1Image> image,
	Point const& targetOffset,
	std::optional<EffectClipSource> const& clip,
	ViewLayerController& viewLayerController
) {
	if (!image) {
		return;
	}

	if (!clip.has_value()) {
		context.getD2dContext()->getDeviceContext()->DrawImage(
			image.Get(),
			{targetOffset.x, targetOffset.y}
		);
		return;
	}

	viewLayerController.pushLayer(context, clip.value(), LayerMode::Normal);
	context.getD2dContext()->getDeviceContext()->DrawImage(
		image.Get(),
		{targetOffset.x, targetOffset.y},
		D2D1_INTERPOLATION_MODE_LINEAR,
		D2D1_COMPOSITE_MODE_SOURCE_COPY
	);
	viewLayerController.popLayer(context);
}
