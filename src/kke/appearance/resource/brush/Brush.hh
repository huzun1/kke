#pragma once

#include <variant>

#include "impl/LinearGradientBrush.hh"
#include "impl/RasterSurfaceBrush.hh"
#include "impl/SolidColorBrush.hh"

namespace kke {
/**
 * @brief Brush variant accepted by fill, stroke, and effect-source APIs.
 */
using Brush = std::variant<SolidColorBrush, LinearGradientBrush, RasterSurfaceBrush>;
} // namespace kke
