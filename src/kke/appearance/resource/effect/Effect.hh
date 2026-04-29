#pragma once

#include <variant>

#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"

namespace kke {
    using Effect = std::variant<ShadowEffect, BlurEffect>;
};
