#include "D2dEngine.hh"
#include "kke/engine/Sources.hh"

using namespace kke;

void D2dEngine::beginDraw(D2dContext const& context, ID2D1Image* renderTarget) {
    renderPass.beginDraw();
}

void D2dEngine::endDraw() {
    renderPass.endDraw();
}

void D2dEngine::clear() {
    renderPass.clear();
}

void D2dEngine::pushTransform(TransformSource const& transform) {
    matrixState.pushTransform(*context, transform);
}

void D2dEngine::popTransform() {
    matrixState.popTransform(*context);
}

std::shared_ptr<Canvas> D2dEngine::createCanvas() {
    return canvasService.createCanvas(*context);
}

void D2dEngine::pushCanvas(std::shared_ptr<Canvas> canvas) {
    canvasService.pushCanvas(*context, canvas);
}

void D2dEngine::popCanvas() {
    canvasService.popCanvas(*context);
}