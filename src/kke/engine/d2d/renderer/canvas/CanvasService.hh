#pragma once

#include <memory>

#include "CanvasRenderTargetStack.hh"
#include "kke/engine/d2d/D2dContext.hh"

namespace kke {
class CanvasService {
    CanvasRenderTargetStack renderTargetStack;

public:
    std::shared_ptr<D2dCanvas> createCanvas(D2dContext const& context);

    void pushCanvas(D2dContext const& context, std::shared_ptr<Canvas> /* std::shared_ptr<D2dCanvas> */ canvas);

    void popCanvas(D2dContext const& context);
};
};