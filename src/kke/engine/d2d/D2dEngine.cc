#include "D2dEngine.hh"

#include <cassert>
#include <memory>

using namespace kke;

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget) {
    std::unique_ptr<D2dContext> d2dContext = std::make_unique<D2dContext>(context);
    this->engineContext->setD2dContext(std::move(d2dContext));

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

void D2dEngine::assertD2dContext() const {
    assert(engineContext != nullptr && "D2dEngineContext is not initialized. Make sure to call beginDraw() before drawing.");
    assert(engineContext->getD2dContext() != nullptr && "D2dContext is not set in D2dEngineContext. Make sure to call beginDraw() with a valid D2dContext.");
}
