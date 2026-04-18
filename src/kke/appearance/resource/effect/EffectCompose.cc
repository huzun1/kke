#include "EffectCompose.hh"

using namespace kke;

void EffectCompose::addEffect(std::shared_ptr<Effect> effect) {
    effects.push_back(effect);
}

std::vector<std::shared_ptr<Effect>> const& EffectCompose::getCompose() const {
    return effects;
}
