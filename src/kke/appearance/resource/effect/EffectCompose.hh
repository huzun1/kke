#pragma once

#include <vector>

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
/**
 * @brief Ordered collection of effects applied as a chain.
 */
class EffectCompose {
	std::vector<Effect> effects;

public:
	/**
	 * @brief Appends an effect to the composition.
	 */
	void addEffect(Effect const& effect);

	/**
	 * @brief Returns the current effect chain in application order.
	 */
	std::vector<Effect> const& getCompose() const;
};
}	// namespace kke
