#include "D2dEngine.hh"

#include <cassert>
#include <memory>

#include "kke/engine/d2d/context/D2dResourceProviders.hh"

using namespace kke;

D2dEngine::D2dEngine() {
	engineContext = std::make_unique<D2dEngineContext>();
	engineContext->setResourceProviders(std::make_unique<D2dResourceProviders>());
}

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget) {
	std::unique_ptr<D2dContext> d2dContext = std::make_unique<D2dContext>(context);
	this->engineContext->setD2dContext(std::move(d2dContext));
	D2D1_SIZE_F viewportSize = renderTarget->GetSize();
	this->engineContext->update(viewportSize);

	renderPass.beginDraw(*this->engineContext, renderTarget);
}

void D2dEngine::endDraw() {
	assertD2dContext();
	renderPass.endDraw(*this->engineContext);
	this->engineContext->releaseD2dContext();
}

void D2dEngine::clear() {
	assertD2dContext();
	renderPass.clear(*this->engineContext);
}

void D2dEngine::pushTransform(TransformSource const& transform) {
	assertD2dContext();
	matrixState.pushTransform(*this->engineContext, transform);
}

void D2dEngine::popTransform() {
	assertD2dContext();
	matrixState.popTransform(*this->engineContext);
}

void D2dEngine::pushLayer(MaskSource const& mask, LayerMode mode) {
	assertD2dContext();
	viewLayerController.pushLayer(*this->engineContext, mask, mode);
}

void D2dEngine::popLayer() {
	assertD2dContext();
	viewLayerController.popLayer(*this->engineContext);
}

std::shared_ptr<Canvas> D2dEngine::createCanvas() {
	assertD2dContext();
	return canvasService.createCanvas(*this->engineContext);
}

void D2dEngine::pushCanvas(std::shared_ptr<Canvas> canvas) {
	assertD2dContext();
	canvasService.pushCanvas(*this->engineContext, canvas);
}

void D2dEngine::popCanvas() {
	assertD2dContext();
	canvasService.popCanvas(*this->engineContext);
}

void D2dEngine::draw(std::shared_ptr<Canvas> canvas, float opacity) {
	assertD2dContext();
	(void)canvas;
	(void)opacity;
}

Scale D2dEngine::getViewportSize() {
	assertD2dContext();
	return measurer.getViewportSize(*this->engineContext);
}

Scale D2dEngine::measureTextSize(Text const& text) {
	assertD2dContext();
	return measurer.measureTextSize(*this->engineContext, text);
}

void D2dEngine::draw(
	StrokeSource const& source,
	Brush const& brush,
	StrokeAppearance const& appearance) {
	assertD2dContext();
	strokeRenderer.draw(*this->engineContext, source, brush, appearance);
}

void D2dEngine::fill(
	FillSource const& source,
	Brush const& brush) {
	assertD2dContext();
	faceRenderer.fill(*this->engineContext, source, brush);
}

std::shared_ptr<Font> D2dEngine::uploadFont(void const* data, size_t size) {
	return engineContext->getResourceProviders()->getFontProvider()->uploadFont(data, size);
}

std::shared_ptr<Texture> D2dEngine::uploadTexture(void const* data, size_t size) {
	assertD2dContext();
	(void)data;
	(void)size;
	return nullptr;
}

void D2dEngine::draw(
	std::shared_ptr<Texture> texture,
	Rect const& destRect,
	float opacity,
	std::optional<Rect> srcRect) {
	assertD2dContext();
	(void)texture;
	(void)destRect;
	(void)opacity;
	(void)srcRect;
}

void D2dEngine::renderEffect(Effect const& effect) {
	assertD2dContext();
	(void)effect;
}

void D2dEngine::renderEffect(EffectSource const& source, Effect const& effect) {
	assertD2dContext();
	(void)source;
	(void)effect;
}

void D2dEngine::renderEffect(EffectCompose const& effect) {
	assertD2dContext();
	(void)effect;
}

void D2dEngine::renderEffect(EffectSource const& source, EffectCompose const& effect) {
	assertD2dContext();
	(void)source;
	(void)effect;
}

void D2dEngine::assertD2dContext() const {
	assert(engineContext != nullptr && "D2dEngineContext is not initialized. Make sure to call beginDraw() before drawing.");
	assert(engineContext->getD2dContext() != nullptr && "D2dContext is not set in D2dEngineContext. Make sure to call beginDraw() with a valid D2dContext.");
}
