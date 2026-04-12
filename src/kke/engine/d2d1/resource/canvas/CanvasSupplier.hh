#pragma once

#include "kke/engine/d2d1/D2d1Context.hh"

namespace kke {
class CanvasSupplier {
    D2d1Context context;

public:
    CanvasSupplier(D2d1Context context);
};
};
