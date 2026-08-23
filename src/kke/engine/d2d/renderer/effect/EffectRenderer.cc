#include "EffectRenderer.hh"

#include <cmath>

#include "kke/appearance/resource/effect/EffectIdentifier.hh"
#include "kke/engine/d2d/renderer/effect/EffectClipBoundsResolver.hh"
#include "kke/engine/d2d/renderer/effect/EffectClipTransformer.hh"
#include "kke/engine/d2d/resource/raster_surface/D2dRasterSurface.hh"

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

std::shared_ptr<Canvas> EffectRenderer::capture(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
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
	if (!sourceImage) {
		deviceContext->SetTransform(activeTransform);
		return nullptr;
	}
	if (viewportClip.has_value()) {
		sourceImage = clipCropper.crop(context, sourceImage, effect, *viewportClip);
	}
	ComPtr<ID2D1Image> effectImage = apply(context, sourceImage, effect);
	if (!effectImage) {
		deviceContext->SetTransform(activeTransform);
		return nullptr;
	}

	std::shared_ptr<D2dCanvas> canvas = canvasService.createCanvas(context);
	if (!canvas || !canvasService.beginCanvas(context, canvas)) {
		deviceContext->SetTransform(activeTransform);
		return nullptr;
	}
	drawImage(context, effectImage, viewportClip, viewLayerController, true);
	if (!canvasService.endCanvas(context) || !canvasService.finishCanvas(canvas)) {
		deviceContext->SetTransform(activeTransform);
		return nullptr;
	}
	deviceContext->SetTransform(activeTransform);
	return canvas;
}

std::optional<CapturedEffect> EffectRenderer::capture(
	D2dEngineContext& context,
	RenderPass& renderPass,
	Effect const& effect,
	EffectClipSource const& clip,
	EffectCaptureOptions const& options,
	RasterSurfaceService& rasterSurfaceService
) {
	if (!std::isfinite(options.rasterScale) || options.rasterScale <= 0.0f) {
		return std::nullopt;
	}
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	D2D1_MATRIX_3X2_F activeTransform;
	deviceContext->GetTransform(&activeTransform);
	EffectClipSource viewportClip = EffectClipTransformer::transform(clip, activeTransform);
	D2D1_RECT_F bounds = EffectClipBoundsResolver::resolve(viewportClip);
	Scale logicalSize{bounds.right - bounds.left, bounds.bottom - bounds.top};
	if (logicalSize.x <= 0.0f || logicalSize.y <= 0.0f) {
		return std::nullopt;
	}

	ComPtr<ID2D1Image> sourceImage =
		renderPass.cycleTargetSnapshot(context, SnapshotOpacityMode::FlattenToOpaqueBlack);
	if (!sourceImage) {
		return std::nullopt;
	}
	sourceImage = clipCropper.crop(context, sourceImage, effect, viewportClip);
	sourceImage = rasterScaler.scaleSource(context, sourceImage, options.rasterScale);
	if (!sourceImage) {
		return std::nullopt;
	}
	Effect scaledEffect = rasterScaler.scaleEffect(effect, options.rasterScale);
	ComPtr<ID2D1Image> effectImage = apply(context, sourceImage, scaledEffect);
	if (!effectImage) {
		return std::nullopt;
	}

	auto surface = options.reusableSurface;
	if (auto d2dSurface = std::dynamic_pointer_cast<D2dRasterSurface>(surface);
		d2dSurface && d2dSurface->getBitmap()) {
		D2D1_SIZE_U pixelSize = d2dSurface->getBitmap()->GetPixelSize();
		float dpiX = 0.0f;
		float dpiY = 0.0f;
		d2dSurface->getBitmap()->GetDpi(&dpiX, &dpiY);
		UINT32 expectedWidth = static_cast<UINT32>(std::ceil(logicalSize.x * options.rasterScale));
		UINT32 expectedHeight = static_cast<UINT32>(std::ceil(logicalSize.y * options.rasterScale));
		if (pixelSize.width != expectedWidth || pixelSize.height != expectedHeight ||
			dpiX != 96.0f || dpiY != 96.0f) {
			surface.reset();
		}
	} else {
		surface.reset();
	}
	if (!surface) {
		surface = rasterSurfaceService.create(context, logicalSize, options.rasterScale, 96.0f);
	}
	auto d2dSurface = std::dynamic_pointer_cast<D2dRasterSurface>(surface);
	if (!d2dSurface || !d2dSurface->getBitmap()) {
		return std::nullopt;
	}
	if (!surface || !rasterSurfaceService.begin(context, surface)) {
		return std::nullopt;
	}
	deviceContext->SetTransform(D2D1::Matrix3x2F(
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		-bounds.left * options.rasterScale,
		-bounds.top * options.rasterScale
	));
	deviceContext->DrawImage(effectImage.Get());
	if (!rasterSurfaceService.end(context)) {
		return std::nullopt;
	}
	return CapturedEffect{
		.surface = std::move(surface),
		.bounds = {{bounds.left, bounds.top}, {bounds.right, bounds.bottom}},
	};
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
