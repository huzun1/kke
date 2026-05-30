#pragma once

#include "kke/appearance/resource/effect/Effect.hh"

namespace kke {
enum class EffectKind {
	Shadow,
	Blur,
	DirectionalBlur,
	ColorMatrix
};

class EffectIdentifier {
public:
	static EffectKind identify(Effect const& effect);
};
}	// namespace kke
