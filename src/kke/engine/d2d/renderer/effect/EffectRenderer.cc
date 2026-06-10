#include "EffectRenderer.hh"

#include <algorithm>

#include "kke/appearance/resource/effect/EffectIdentifier.hh"
#include "kke/engine/d2d/renderer/effect/EffectClipBoundsResolver.hh"
#include "kke/engine/d2d/renderer/effect/EffectPaddingEstimator.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

void EffectRenderer::beginDraw() {
	commandListSnapshotter.beginFrame();
}

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	if (clip.has_value()) {
		renderClipEffect(context, renderPass, effect, clip.value(), viewLayerController);
		return;
	}

	ComPtr<ID2D1Image> sourceImage = renderPass.cycleTargetSnapshot(context);
	if (!sourceImage) {
		return;
	}

	drawImage(context, apply(context, sourceImage, effect), std::nullopt, viewLayerController);
}

void EffectRenderer::render(
	D2dEngineContext& context,
	RenderPass& renderPass,
	EffectCompose const& effectCompose,
	std::optional<EffectClipSource> clip,
	ViewLayerController& viewLayerController
) {
	if (clip.has_value()) {
		renderClipEffect(context, renderPass, effectCompose, clip.value(), viewLayerController);
		return;
	}

	ComPtr<ID2D1Image> sourceImage = renderPass.cycleTargetSnapshot(context);
	if (!sourceImage) {
		return;
	}

	drawImage(
		context,
		apply(context, sourceImage, effectCompose),
		std::nullopt,
		viewLayerController
	);
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

void EffectRenderer::renderClipEffect(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	EffectClipSource const& clip,
	ViewLayerController& viewLayerController
) {
	ID2D1Bitmap* renderTarget = renderPass.getRenderTarget();
	if (!renderTarget) {
		return;
	}

	ComPtr<ID2D1Bitmap1> sourceImage = renderPass.cycleTargetSnapshot(context);
	if (!sourceImage) {
		return;
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_RECT_F effectBounds = resolveEffectBounds(renderTarget, effect, clip);

	ComPtr<ID2D1Bitmap1> localSourceImage = commandListSnapshotter.snapshotRegion(
		deviceContext,
		sourceImage.Get(),
		renderTarget,
		effectBounds
	);
	if (localSourceImage) {
		ComPtr<ID2D1Image> effectImage = apply(context, localSourceImage, effect);
		drawImage(
			context,
			effectImage,
			resolveImageDrawOffset(context, effectImage, {effectBounds.left, effectBounds.top}),
			clip,
			viewLayerController
		);
		return;
	}

	ComPtr<ID2D1Image> effectSourceImage = effectClipCropper.crop(context, sourceImage, effect, clip);
	drawImage(context, apply(context, effectSourceImage, effect), clip, viewLayerController);
}

void EffectRenderer::renderClipEffect(
	D2dEngineContext& context,
	RenderPass& renderPass,
	EffectCompose const& effectCompose,
	EffectClipSource const& clip,
	ViewLayerController& viewLayerController
) {
	ID2D1Bitmap* renderTarget = renderPass.getRenderTarget();
	if (!renderTarget) {
		return;
	}

	ComPtr<ID2D1Bitmap1> sourceImage = renderPass.cycleTargetSnapshot(context);
	if (!sourceImage) {
		return;
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_RECT_F effectBounds = resolveEffectBounds(renderTarget, effectCompose, clip);

	ComPtr<ID2D1Bitmap1> localSourceImage = commandListSnapshotter.snapshotRegion(
		deviceContext,
		sourceImage.Get(),
		renderTarget,
		effectBounds
	);
	if (localSourceImage) {
		ComPtr<ID2D1Image> effectImage = apply(context, localSourceImage, effectCompose);
		drawImage(
			context,
			effectImage,
			resolveImageDrawOffset(context, effectImage, {effectBounds.left, effectBounds.top}),
			clip,
			viewLayerController
		);
		return;
	}

	ComPtr<ID2D1Image> effectSourceImage =
		effectClipCropper.crop(context, sourceImage, effectCompose, clip);
	drawImage(context, apply(context, effectSourceImage, effectCompose), clip, viewLayerController);
}

D2D1_RECT_F
EffectRenderer::resolveEffectBounds(ID2D1Bitmap* renderTarget, Effect const& effect, EffectClipSource const& clip)
	const {
	D2D1_RECT_F effectBounds = EffectClipBoundsResolver::resolve(clip);
	float padding = EffectPaddingEstimator::estimate(effect);
	D2D1_SIZE_F targetSize = renderTarget->GetSize();

	return D2D1::RectF(
		std::max(0.0f, effectBounds.left - padding),
		std::max(0.0f, effectBounds.top - padding),
		std::min(targetSize.width, effectBounds.right + padding),
		std::min(targetSize.height, effectBounds.bottom + padding)
	);
}

D2D1_RECT_F EffectRenderer::resolveEffectBounds(
	ID2D1Bitmap* renderTarget, EffectCompose const& effectCompose, EffectClipSource const& clip
) const {
	D2D1_RECT_F effectBounds = EffectClipBoundsResolver::resolve(clip);
	float padding = EffectPaddingEstimator::estimate(effectCompose);
	D2D1_SIZE_F targetSize = renderTarget->GetSize();

	return D2D1::RectF(
		std::max(0.0f, effectBounds.left - padding),
		std::max(0.0f, effectBounds.top - padding),
		std::min(targetSize.width, effectBounds.right + padding),
		std::min(targetSize.height, effectBounds.bottom + padding)
	);
}

Point EffectRenderer::resolveImageDrawOffset(
	D2dEngineContext& context, ComPtr<ID2D1Image> image, Point const& baseOffset
) const {
	if (!image) {
		return baseOffset;
	}

	D2D1_RECT_F imageBounds;
	HRESULT boundsResult =
		context.getD2dContext()->getDeviceContext()->GetImageLocalBounds(image.Get(), &imageBounds);
	if (FAILED(boundsResult)) {
		return baseOffset;
	}

	return {baseOffset.x + imageBounds.left, baseOffset.y + imageBounds.top};
}

void EffectRenderer::drawImage(
	D2dEngineContext const& context,
	ComPtr<ID2D1Image> image,
	std::optional<EffectClipSource> const& clip,
	ViewLayerController& viewLayerController
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
		D2D1_COMPOSITE_MODE_SOURCE_COPY
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
