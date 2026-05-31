#pragma once

#include <optional>

#include "kke/geometry/shapes/Rect.hh"

#include "TextureInterpolation.hh"

namespace kke {
/**
 * @brief Controls how a texture is sampled when drawn.
 */
struct TextureDrawAppearance {
	/**
	 * @brief Overall alpha multiplier applied during drawing.
	 */
	float opacity = 1.0f;

	/**
	 * @brief Sampling mode used when scaling the texture.
	 */
	TextureInterpolation interpolation = TextureInterpolation::Linear;

	/**
	 * @brief Optional source rectangle to crop from the texture before drawing.
	 */
	std::optional<Rect> srcRect = std::nullopt;
};
} // namespace kke
