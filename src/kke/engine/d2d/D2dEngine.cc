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
#include "kke/engine/d2d/renderer/view/MatrixState.hh"
#include "kke/engine/d2d/renderer/view/ViewLayerController.hh"

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
	EffectRenderer effectRenderer;
};

D2dEngine::D2dEngine() : impl(std::make_unique<Impl>()) {
	impl->engineContext = std::make_unique<D2dEngineContext>();
	impl->engineContext->setResourceProviders(std::make_unique<D2dResourceProviders>());
}

D2dEngine::~D2dEngine() = default;

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget) {
	std::unique_ptr<D2dContext> d2dContext = std::make_unique<D2dContext>(context);
	impl->engineContext->setD2dContext(std::move(d2dContext));
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

std::shared_ptr<Canvas> D2dEngine::createCanvas() {
	assertD2dContext();
	return impl->canvasService.createCanvas(*impl->engineContext);
}

void D2dEngine::pushCanvas(std::shared_ptr<Canvas> canvas) {
	assertD2dContext();
	impl->canvasService.pushCanvas(*impl->engineContext, canvas);
}

void D2dEngine::popCanvas() {
	assertD2dContext();
	impl->canvasService.popCanvas(*impl->engineContext);
}

void D2dEngine::draw(std::shared_ptr<Canvas> canvas, float opacity) {
	assertD2dContext();
	impl->canvasService.drawCanvas(*impl->engineContext, canvas, opacity);
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
	impl->strokeRenderer.draw(*impl->engineContext, source, brush, appearance);
}

void D2dEngine::fill(FillSource const& source, Brush const& brush) {
	assertD2dContext();
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
	impl->textureRenderer.draw(*impl->engineContext, texture, destRect, appearance);
}

void D2dEngine::renderEffect(Effect const& effect, std::optional<EffectClipSource> clip) {
	assertD2dContext();
	impl->effectRenderer
		.render(*impl->engineContext, impl->renderPass, effect, clip, impl->viewLayerController);
}

void D2dEngine::renderEffect(
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	Effect const& effect,
	std::optional<EffectClipSource> clip
) {
	assertD2dContext();
	impl->effectRenderer.render(
		*impl->engineContext, source, sourceAppearance, effect, clip, impl->viewLayerController
	);
}

void D2dEngine::renderEffect(EffectCompose const& effect, std::optional<EffectClipSource> clip) {
	assertD2dContext();
	impl->effectRenderer.render(
		*impl->engineContext, impl->renderPass, effect, clip, impl->viewLayerController
	);
}

void D2dEngine::renderEffect(
	EffectSource const& source,
	EffectSourceAppearance const& sourceAppearance,
	EffectCompose const& effect,
	std::optional<EffectClipSource> clip
) {
	assertD2dContext();
	impl->effectRenderer.render(
		*impl->engineContext, source, sourceAppearance, effect, clip, impl->viewLayerController
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
