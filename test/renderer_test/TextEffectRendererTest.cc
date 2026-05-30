#include "TextEffectRendererTest.hh"

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/font/FontWeight.hh"

namespace application {
namespace renderer_test {
void TextEffectRendererTest::render() {
	kke::Brush outline = kke::SolidColorBrush({0.92f, 0.94f, 0.96f, 1.0f});

	kke::Text label{
		L"Space Grotesk via DWrite",
		{120.0f, 500.0f},
		{"Space Grotesk", 36.0f, kke::FontWeight::BOLD}
	};

	kke::EffectSourceAppearance labelAppearance;
	labelAppearance.brush = outline;

	engine().renderEffect(
		label,
		labelAppearance,
		kke::ShadowEffect{{8.0f, 10.0f}, 8.0f, {0.0f, 0.0f, 0.0f, 0.85f}, kke::ShadowMode::ShadowWithSource});
}
}
}
