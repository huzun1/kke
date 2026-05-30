#pragma once

#include <optional>

#include "kke/geometry/shapes/Rect.hh"

#include "TextureInterpolation.hh"

namespace kke {
struct TextureDrawAppearance {
	float opacity = 1.0f;
	TextureInterpolation interpolation = TextureInterpolation::Linear;
	std::optional<Rect> srcRect = std::nullopt;
};
}	// namespace kke
