#pragma once

#include <vector>

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
class EffectCompose {
	std::vector<Effect> effects;

public:
	void addEffect(Effect const& effect);

	std::vector<Effect> const& getCompose() const;
};
}	// namespace kke
