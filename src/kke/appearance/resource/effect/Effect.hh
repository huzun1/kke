#pragma once

#include <variant>

#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/color_matrix/ColorMatrixEffect.hh"
#include "kke/appearance/resource/effect/directional_blur/DirectionalBlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"

namespace kke {
/**
 * @brief Variant of all built-in effects that can be rendered by the engine.
 */
using Effect = std::variant<ShadowEffect, BlurEffect, DirectionalBlurEffect, ColorMatrixEffect>;
} // namespace kke
