#include "D2dEngine.hh"

#include <cassert>
#include <memory>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/context/D2dResourceProviders.hh"
#include "kke/engine/d2d/renderer/Measurer.hh"
#include "kke/engine/d2d/renderer/RenderPass.hh"
#include "kke/engine/d2d/renderer/canvas/CanvasService.hh"
#include "kke/engine/d2d/renderer/effect/EffectRenderer.hh"
#include "kke/engine/d2d/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d/renderer/painting/StrokeRenderer.hh"
#include "kke/engine/d2d/renderer/painting/TextureRenderer.hh"
#include "kke/engine/d2d/renderer/raster_surface/RasterSurfaceService.hh"
#include "kke/engine/d2d/renderer/view/MatrixState.hh"
#include "kke/engine/d2d/renderer/view/ViewLayerController.hh"
#include "kke/engine/d2d/resource/raster_surface/D2dRasterSurface.hh"

using namespace kke;

struct D2dEngine::Impl {
	std::unique_ptr<D2dEngineContext> engineContext;

	RenderPass renderPass;
	MatrixState matrixState;
	ViewLayerController viewLayerController;
	Measurer measurer;
	CanvasService canvasService;
	FaceRenderer faceRenderer;
	StrokeRenderer strokeRenderer;
	TextureRenderer textureRenderer;
	RasterSurfaceService rasterSurfaceService;
	EffectRenderer effectRenderer;
};

D2dEngine::D2dEngine() : impl(std::make_unique<Impl>()) {
	impl->engineContext = std::make_unique<D2dEngineContext>();
	impl->engineContext->setResourceProviders(std::make_unique<D2dResourceProviders>());
}

D2dEngine::~D2dEngine() = default;

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget) {
	impl->engineContext->setD2dContext(context);
	D2D1_SIZE_F viewportSize = renderTarget->GetSize();
	impl->engineContext->update(viewportSize);

	impl->renderPass.beginDraw(*impl->engineContext, renderTarget);
}

void D2dEngine::endDraw() {
	assertD2dContext();
	impl->renderPass.endDraw(*impl->engineContext);
	impl->engineContext->releaseD2dContext();
}

void D2dEngine::clear() {
	assertD2dContext();
	impl->renderPass.clear(*impl->engineContext);
}

void D2dEngine::pushTransform(TransformSource const& transform) {
	assertD2dContext();
	impl->matrixState.pushTransform(*impl->engineContext, transform);
}

void D2dEngine::popTransform() {
	assertD2dContext();
	impl->matrixState.popTransform(*impl->engineContext);
}

void D2dEngine::pushLayer(MaskSource const& mask, LayerMode mode) {
	assertD2dContext();
	impl->viewLayerController.pushLayer(*impl->engineContext, mask, mode);
}

void D2dEngine::popLayer() {
	assertD2dContext();
	impl->viewLayerController.popLayer(*impl->engineContext);
}

void D2dEngine::pushAxisAlignedClip(Rect const& rect) {
	assertD2dContext();
	impl->engineContext->getD2dContext()->getDeviceContext()->PushAxisAlignedClip(
		D2D1::RectF(rect.min.x, rect.min.y, rect.max.x, rect.max.y),
		D2D1_ANTIALIAS_MODE_ALIASED
	);
}

void D2dEngine::popAxisAlignedClip() {
	assertD2dContext();
	impl->engineContext->getD2dContext()->getDeviceContext()->PopAxisAlignedClip();
}

std::shared_ptr<Canvas> D2dEngine::createCanvas() {
	assertD2dContext();
	return impl->canvasService.createCanvas(*impl->engineContext);
}

bool D2dEngine::beginCanvas(std::shared_ptr<Canvas> canvas) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	if (!impl->canvasService.beginCanvas(*impl->engineContext, canvas)) {
		return false;
	}
	impl->matrixState.beginCanvas(*impl->engineContext);
	return true;
}

bool D2dEngine::endCanvas() {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	if (!impl->canvasService.endCanvas(*impl->engineContext)) {
		return false;
	}
	impl->matrixState.endCanvas(*impl->engineContext);
	return true;
}

bool D2dEngine::finishCanvas(std::shared_ptr<Canvas> canvas) {
	assertD2dContext();
	return impl->canvasService.finishCanvas(canvas);
}

void D2dEngine::draw(std::shared_ptr<Canvas> canvas, float opacity) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->canvasService.drawCanvas(*impl->engineContext, canvas, opacity);
}

std::shared_ptr<RasterSurface>
D2dEngine::createRasterSurface(Scale const& logicalSize, float rasterScale) {
	assertD2dContext();
	return impl->rasterSurfaceService.create(*impl->engineContext, logicalSize, rasterScale);
}

bool D2dEngine::beginRasterSurface(
	std::shared_ptr<RasterSurface> surface, Point const& logicalOrigin
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	if (!impl->rasterSurfaceService.begin(*impl->engineContext, surface)) {
		return false;
	}
	impl->matrixState.beginCanvas(*impl->engineContext);
	impl->matrixState.pushTransform(
		*impl->engineContext,
		TransformSource{Translation({-logicalOrigin.x, -logicalOrigin.y})}
	);
	return true;
}

bool D2dEngine::endRasterSurface() {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->matrixState.popTransform(*impl->engineContext);
	if (!impl->rasterSurfaceService.end(*impl->engineContext)) {
		impl->matrixState.endCanvas(*impl->engineContext);
		return false;
	}
	impl->matrixState.endCanvas(*impl->engineContext);
	return true;
}

void D2dEngine::draw(
	std::shared_ptr<RasterSurface> surface, Rect const& destination, float opacity
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->rasterSurfaceService.draw(*impl->engineContext, surface, destination, opacity);
}

Scale D2dEngine::getViewportSize() {
	assertD2dContext();
	return impl->measurer.getViewportSize(*impl->engineContext);
}

Scale D2dEngine::measureTextSize(Text const& text) {
	assertD2dContext();
	return impl->measurer.measureTextSize(*impl->engineContext, text);
}

void D2dEngine::draw(
	StrokeSource const& source, Brush const& brush, StrokeAppearance const& appearance
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->strokeRenderer.draw(*impl->engineContext, source, brush, appearance);
}

void D2dEngine::fill(FillSource const& source, Brush const& brush) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->faceRenderer.fill(*impl->engineContext, source, brush);
}

std::shared_ptr<Font> D2dEngine::uploadFont(void const* data, size_t size) {
	return impl->engineContext->getResourceProviders()->getFontProvider()->uploadFont(data, size);
}

std::shared_ptr<Texture> D2dEngine::uploadTexture(void const* data, size_t size) {
	assertD2dContext();
	return impl->engineContext->getResourceProviders()->getTextureProvider()->uploadTexture(
		*impl->engineContext->getD2dContext(),
		data,
		size
	);
}

std::shared_ptr<Texture> D2dEngine::uploadTexture(RawTextureData const& data) {
	assertD2dContext();
	return impl->engineContext->getResourceProviders()->getTextureProvider()->uploadTexture(
		*impl->engineContext->getD2dContext(),
		data
	);
}

void D2dEngine::draw(
	std::shared_ptr<Texture> texture, Rect const& destRect, TextureDrawAppearance const& appearance
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->textureRenderer.draw(*impl->engineContext, texture, destRect, appearance);
}

void D2dEngine::renderEffect(
	Effect const& effect, std::optional<EffectClipSource> clip, float opacity
) {
	assertD2dContext();
	impl->effectRenderer.render(
		*impl->engineContext,
		impl->renderPass,
		effect,
		clip,
		opacity,
		impl->viewLayerController
	);
	impl->renderPass.invalidateCachedTargetSnapshot();
}

std::shared_ptr<Canvas>
D2dEngine::captureEffect(Effect const& effect, std::optional<EffectClipSource> clip) {
	assertD2dContext();
	std::shared_ptr<Canvas> canvas = impl->effectRenderer.capture(
		*impl->engineContext,
		impl->renderPass,
		effect,
		clip,
		impl->viewLayerController
	);
	impl->renderPass.invalidateCachedTargetSnapshot();
	return canvas;
}

std::optional<CapturedEffect> D2dEngine::captureEffect(
	Effect const& effect, EffectClipSource const& clip, EffectCaptureOptions const& options
) {
	assertD2dContext();
	auto captured = impl->effectRenderer.capture(
		*impl->engineContext,
		impl->renderPass,
		effect,
		clip,
		options,
		impl->rasterSurfaceService
	);
	impl->renderPass.invalidateCachedTargetSnapshot();
	return captured;
}

void D2dEngine::renderEffect(
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	Effect const& effect,
	std::optional<EffectClipSource> clip
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->effectRenderer.render(
		*impl->engineContext,
		source,
		sourceAppearance,
		effect,
		clip,
		impl->viewLayerController
	);
}

void D2dEngine::renderEffect(EffectCompose const& effect, std::optional<EffectClipSource> clip) {
	assertD2dContext();
	impl->effectRenderer
		.render(*impl->engineContext, impl->renderPass, effect, clip, impl->viewLayerController);
	impl->renderPass.invalidateCachedTargetSnapshot();
}

void D2dEngine::renderEffect(
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	EffectCompose const& effect,
	std::optional<EffectClipSource> clip
) {
	assertD2dContext();
	impl->renderPass.invalidateCachedTargetSnapshot();
	impl->effectRenderer.render(
		*impl->engineContext,
		source,
		sourceAppearance,
		effect,
		clip,
		impl->viewLayerController
	);
}

void D2dEngine::assertD2dContext() const {
	assert(
		impl->engineContext != nullptr &&
		"D2dEngineContext is not initialized. Make sure to call beginDraw() before drawing."
	);
	assert(
		impl->engineContext->getD2dContext() != nullptr &&
		"D2dContext is not set in D2dEngineContext. Make sure to call beginDraw() with a valid "
		"D2dContext."
	);
}
