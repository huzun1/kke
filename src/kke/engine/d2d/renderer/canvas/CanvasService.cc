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

void CanvasService::drawCanvas(D2dEngineContext const& context, std::shared_ptr<Canvas> canvas, float opacity) {
    std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(canvas);
    if (!d2dCanvas) {
        // TODO: Log error - invalid canvas type
        return;
    }

    if (!d2dCanvas->close()) {
        // TODO: Log error - failed to close canvas
        return;
    }

    ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
    if (opacity >= 1.0f) {
        deviceContext->DrawImage(d2dCanvas->getCommandList().Get());
        return;
    }

    D2D1_LAYER_PARAMETERS1 layerParameters = D2D1::LayerParameters1(
        D2D1::InfiniteRect(),
        nullptr,
        D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
        D2D1::IdentityMatrix(),
        opacity);
    deviceContext->PushLayer(layerParameters, nullptr);
    deviceContext->DrawImage(d2dCanvas->getCommandList().Get());
    deviceContext->PopLayer();
}
