#include "CanvasService.hh"
#include <memory>

#include "kke/engine/d2d/resource/canvas/CanvasFactory.hh"

using namespace kke;

std::shared_ptr<D2dCanvas> CanvasService::createCanvas(D2dEngineContext const& context) {
    return CanvasFactory::createCanvas(context);
}

void CanvasService::pushCanvas(D2dEngineContext const& context, std::shared_ptr<Canvas> canvas) {
    std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(canvas);
    if (!d2dCanvas) {
        // TODO: Log error - invalid canvas type
        return;
    }
    renderTargetStack.pushCanvas(context, d2dCanvas);
}

void CanvasService::popCanvas(D2dEngineContext const& context) {
    renderTargetStack.popCanvas(context);
}
