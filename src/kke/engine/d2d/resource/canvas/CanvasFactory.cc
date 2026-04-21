#include "CanvasFactory.hh"

#include <memory>
#include "D2dCanvas.hh"

using namespace kke;
using namespace Microsoft::WRL;

std::shared_ptr<D2dCanvas> CanvasFactory::createCanvas(D2dContext const& context) {
    ComPtr<ID2D1CommandList> commandList;
    HRESULT commandListResult = context.getDeviceContext()->CreateCommandList(&commandList);
    if (FAILED(commandListResult)) {
        // TODO: Log error
        return nullptr;
    }
    return std::make_shared<D2dCanvas>(commandList);
}
