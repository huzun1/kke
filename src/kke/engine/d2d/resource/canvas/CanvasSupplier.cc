#include "CanvasSupplier.hh"

#include <wrl/client.h>
#include <memory>
#include "D2dCanvas.hh"

using namespace kke;
using namespace Microsoft::WRL;

std::shared_ptr<D2dCanvas> CanvasSupplier::getOrCreateCanvas(D2dContext const& context) {
    std::shared_ptr<D2dCanvas> canvas = getCanvasFromCache(context);
    if (canvas) {
        return canvas;
    }
    return createCanvas(context);
}

void CanvasSupplier::releaseCanvas(std::shared_ptr<D2dCanvas> canvas) {
    if (!canvas) {
        // TODO: Log error
        return;
    }
    commandListCache.release(canvas);
}


std::shared_ptr<D2dCanvas> CanvasSupplier::getCanvasFromCache(D2dContext const& context) {
    std::shared_ptr<D2dCanvas> canvas = commandListCache.tryUse();
    if (canvas) {
        return canvas;
    }
    return createCanvas(context);
}

std::shared_ptr<D2dCanvas> CanvasSupplier::createCanvas(D2dContext const& context) {
    ComPtr<ID2D1CommandList> commandList;
    HRESULT commandListResult = context.getDeviceContext()->CreateCommandList(&commandList);
    if (FAILED(commandListResult)) {
        // TODO: Log error
        return nullptr;
    }
    std::shared_ptr<D2dCanvas> canvas = std::make_shared<D2dCanvas>(commandList);
    commandListCache.putAndUse(canvas);
    return canvas;
}
