#include "D2dEngine.hh"

using namespace kke;

void D2dEngine::beginDraw() {
    renderPass.beginDraw();
}

void D2dEngine::endDraw() {
    renderPass.endDraw();
}

void D2dEngine::clear() {
    renderPass.clear();
}

void D2dEngine::pushTransform(Translation const& translate) {
    matrixState.pushTransform(*context, translate);
}

void D2dEngine::pushTransform(Scaling const& scale) {
    matrixState.pushTransform(*context, scale);
}

void D2dEngine::pushTransform(Rotation const& rotate) {
    matrixState.pushTransform(*context, rotate);
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