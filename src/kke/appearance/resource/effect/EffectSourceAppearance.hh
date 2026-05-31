#pragma once

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceDrawMode.hh"

namespace kke {
/**
 * @brief Describes how an effect source is drawn before the effect is applied.
 */
struct EffectSourceAppearance {
	/**
	 * @brief Brush used to rasterize the source geometry or text.
	 */
	Brush brush{SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f})};

	/**
	 * @brief Whether the source is filled or stroked before effect processing.
	 */
	EffectSourceDrawMode drawMode = EffectSourceDrawMode::Fill;

	/**
	 * @brief Stroke thickness used when `drawMode` is `Stroke`.
	 */
	float strokeWidth = 1.0f;
};
} // namespace kke
