#pragma once

#include "kke/resources/effect/blur/BlurBorderMode.hh"
#include "kke/resources/effect/blur/BlurOptimization.hh"

namespace kke {
struct BlurAppearance {
    float radius;
    BlurBorderMode borderMode;
    BlurOptimization optimization;    
};
};
