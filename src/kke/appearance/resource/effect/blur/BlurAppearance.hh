#pragma once

#include "BlurBorderMode.hh"
#include "BlurOptimization.hh"

namespace kke {
struct BlurAppearance {
    float radius;
    BlurBorderMode borderMode;
    BlurOptimization optimization;    
};
};
