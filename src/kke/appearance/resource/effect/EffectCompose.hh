#pragma once

#include <memory>
#include <vector>

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
/**
 * @brief Represents a composition of multiple effects.
 *
 * This class allows combining multiple Effect instances
 * into a single ordered sequence, which can be applied
 * as a composed effect.
 */
class EffectCompose {
	std::vector<std::shared_ptr<Effect>> effects;

public:
    /**
     * @brief Adds an effect to the composition.
     *
     * The effect will be appended to the internal sequence
     * and applied in order during rendering.
     *
     * @param effect Shared pointer to the effect to add.
     */
    void addEffect(std::shared_ptr<Effect> effect);

    /**
     * @brief Retrieves the composed effects.
     *
     * @return A constant reference to the internal list of effects.
     */
    std::vector<std::shared_ptr<Effect>> const& getCompose() const;
};
};	// namespace kke
