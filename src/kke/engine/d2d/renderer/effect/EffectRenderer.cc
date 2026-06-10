#include "EffectRenderer.hh"

#include <algorithm>

#include "kke/appearance/resource/effect/EffectIdentifier.hh"
#include "kke/engine/d2d/renderer/effect/EffectClipBoundsResolver.hh"

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

	ComPtr<ID2D1Image> effectImage =
		localSourceImage ? apply(context, localSourceImage, effect) : apply(context, sourceImage, effect);
	if (!effectImage) {
		return;
	}

	ComPtr<ID2D1Bitmap1> compositeBitmap = acquireClipCompositeBitmap(deviceContext, renderTarget);
	if (!compositeBitmap) {
		return;
	}

	ComPtr<ID2D1Image> previousTarget;
	deviceContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	deviceContext->GetTransform(&previousTransform);

	deviceContext->SetTarget(compositeBitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();
	deviceContext->DrawImage(sourceImage.Get());
	if (localSourceImage) {
		drawImage(
			context,
			effectImage,
			{effectBounds.left, effectBounds.top},
			clip,
			viewLayerController
		);
	} else {
		drawImage(context, effectImage, clip, viewLayerController);
	}

	deviceContext->SetTransform(previousTransform);
	deviceContext->SetTarget(previousTarget.Get());

	deviceContext->Clear();
	deviceContext->DrawBitmap(compositeBitmap.Get());
	renderPass.setCachedTargetSnapshot(compositeBitmap);
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

	ComPtr<ID2D1Image> effectImage = localSourceImage ?
		apply(context, localSourceImage, effectCompose) :
		apply(context, sourceImage, effectCompose);
	if (!effectImage) {
		return;
	}

	ComPtr<ID2D1Bitmap1> compositeBitmap = acquireClipCompositeBitmap(deviceContext, renderTarget);
	if (!compositeBitmap) {
		return;
	}

	ComPtr<ID2D1Image> previousTarget;
	deviceContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	deviceContext->GetTransform(&previousTransform);

	deviceContext->SetTarget(compositeBitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();
	deviceContext->DrawImage(sourceImage.Get());
	if (localSourceImage) {
		drawImage(
			context,
			effectImage,
			{effectBounds.left, effectBounds.top},
			clip,
			viewLayerController
		);
	} else {
		drawImage(context, effectImage, clip, viewLayerController);
	}

	deviceContext->SetTransform(previousTransform);
	deviceContext->SetTarget(previousTarget.Get());

	deviceContext->Clear();
	deviceContext->DrawBitmap(compositeBitmap.Get());
	renderPass.setCachedTargetSnapshot(compositeBitmap);
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

ComPtr<ID2D1Bitmap1> EffectRenderer::acquireClipCompositeBitmap(
	ID2D1DeviceContext* deviceContext, ID2D1Bitmap* renderTarget
) {
	if (!deviceContext || !renderTarget) {
		return nullptr;
	}

	float dpiX, dpiY;
	renderTarget->GetDpi(&dpiX, &dpiY);

	D2D1_SIZE_U pixelSize = renderTarget->GetPixelSize();
	D2D1_PIXEL_FORMAT pixelFormat = renderTarget->GetPixelFormat();

	constexpr size_t clipCompositeBitmapCacheCount = 2;
	ClipCompositeBitmapCache& cacheEntry =
		clipCompositeBitmapCaches[clipCompositeBitmapCacheIndex % clipCompositeBitmapCacheCount];
	clipCompositeBitmapCacheIndex++;

	if (
		cacheEntry.bitmap &&
		cacheEntry.pixelSize.width == pixelSize.width &&
		cacheEntry.pixelSize.height == pixelSize.height &&
		cacheEntry.pixelFormat.format == pixelFormat.format &&
		cacheEntry.pixelFormat.alphaMode == pixelFormat.alphaMode &&
		cacheEntry.dpiX == dpiX &&
		cacheEntry.dpiY == dpiY
	) {
		return cacheEntry.bitmap;
	}

	D2D1_BITMAP_PROPERTIES1 properties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, pixelFormat, dpiX, dpiY);

	ComPtr<ID2D1Bitmap1> bitmap;
	HRESULT createResult = deviceContext->CreateBitmap(pixelSize, nullptr, 0, properties, &bitmap);
	if (FAILED(createResult) || !bitmap) {
		return nullptr;
	}

	cacheEntry.bitmap = bitmap;
	cacheEntry.pixelSize = pixelSize;
	cacheEntry.pixelFormat = pixelFormat;
	cacheEntry.dpiX = dpiX;
	cacheEntry.dpiY = dpiY;
	return bitmap;
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
