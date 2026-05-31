#pragma once

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
/**
 * @brief Discriminator for the concrete type stored in an `Effect`.
 */
enum class EffectKind { Shadow, Blur, DirectionalBlur, ColorMatrix };

/**
 * @brief Utility that maps an `Effect` variant to its corresponding `EffectKind`.
 */
class EffectIdentifier {
  public:
	/**
	 * @brief Returns the concrete effect kind currently stored in `effect`.
	 */
	static EffectKind identify(Effect const& effect);
};
} // namespace kke
