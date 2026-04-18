#pragma once

#include <memory>
#include <vector>

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
class EffectCompose {
	std::vector<std::shared_ptr<Effect>> effects;

public:
    void addEffect(std::shared_ptr<Effect> effect);

    std::vector<std::shared_ptr<Effect>> const& getCompose() const;
};
};	// namespace kke
