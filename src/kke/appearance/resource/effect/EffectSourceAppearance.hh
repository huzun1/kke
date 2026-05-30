#pragma once

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceDrawMode.hh"

namespace kke {
struct EffectSourceAppearance {
	Brush brush {SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f})};
	EffectSourceDrawMode drawMode = EffectSourceDrawMode::Fill;
	float strokeWidth = 1.0f;
};
}	// namespace kke
