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

