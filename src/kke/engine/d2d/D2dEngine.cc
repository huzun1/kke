#include "D2dEngine.hh"
#include <cassert>
#include "kke/engine/Sources.hh"

using namespace kke;

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget) {
    this->context = std::make_unique<D2dContext>(context);
    renderPass.beginDraw(*this->context, renderTarget);
}

void D2dEngine::endDraw() {
    assertContext();
    renderPass.endDraw(*this->context);
    this->context.reset();
}

void D2dEngine::clear() {
    assertContext();
    renderPass.clear(*this->context);
}

void D2dEngine::pushTransform(TransformSource const& transform) {
    assertContext();
    matrixState.pushTransform(*this->context, transform);
}

void D2dEngine::popTransform() {
    assertContext();
    matrixState.popTransform(*this->context);
}

std::shared_ptr<Canvas> D2dEngine::createCanvas() {
    assertContext();
    return canvasService.createCanvas(*this->context);
}

void D2dEngine::pushCanvas(std::shared_ptr<Canvas> canvas) {
    assertContext();
    canvasService.pushCanvas(*this->context, canvas);
}

void D2dEngine::popCanvas() {
    assertContext();
    canvasService.popCanvas(*this->context);
}

void D2dEngine::assertContext() const {
    assert(context != nullptr && "D2dContext is not initialized. Make sure to call beginDraw() before drawing.");
}