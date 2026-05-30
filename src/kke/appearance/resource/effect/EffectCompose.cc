#include "EffectCompose.hh"

using namespace kke;

void EffectCompose::addEffect(Effect const& effect) {
	effects.push_back(effect);
}

std::vector<Effect> const& EffectCompose::getCompose() const {
	return effects;
}
