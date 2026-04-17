#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>

#include "kke/appearance/Canvas.hh"

using namespace Microsoft::WRL;

namespace kke {
class D2dCanvas : public Canvas {
    ComPtr<ID2D1CommandList> commandList;

public:
    D2dCanvas(ComPtr<ID2D1CommandList> commandList);

    ComPtr<ID2D1CommandList> getCommandList() const;
};
};
