#include "CanvasFactory.hh"

#include <memory>

#include "kke/engine/d2d/context/D2dEngineContext.hh"

using namespace kke;
using namespace Microsoft::WRL;

std::shared_ptr<D2dCanvas> CanvasFactory::createCanvas(D2dEngineContext const& context) {
    D2dContext* d2dContext = context.getD2dContext();

    ComPtr<ID2D1CommandList> commandList;
    HRESULT commandListResult = d2dContext->getDeviceContext()->CreateCommandList(&commandList);
    if (FAILED(commandListResult)) {
        // TODO: Log error
        return nullptr;
    }
    return std::make_shared<D2dCanvas>(commandList);
}
