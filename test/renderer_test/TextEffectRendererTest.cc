#include "TextEffectRendererTest.hh"

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void TextEffectRendererTest::render() {
	kke::Brush background = kke::SolidColorBrush({0.86f, 0.89f, 0.94f, 1.0f});
	kke::Brush panelFill = kke::SolidColorBrush({0.14f, 0.18f, 0.25f, 0.96f});
	kke::Brush outline = kke::SolidColorBrush({0.95f, 0.97f, 0.99f, 1.0f});
	kke::Brush accent = kke::SolidColorBrush({0.45f, 0.79f, 1.0f, 1.0f});
	kke::Brush warmAccent = kke::SolidColorBrush({1.0f, 0.73f, 0.32f, 1.0f});

	engine().fill(kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}, background);
	engine().fill(kke::Rect{{72.0f, 84.0f}, {1208.0f, 636.0f}}, panelFill);

	kke::Text title{
		L"ShadowWithSource text samples",
		{108.0f, 126.0f},
		{"Space Grotesk", 24.0f, kke::FontWeight::BOLD}
	};

	engine().fill(title, outline);

	kke::EffectSourceAppearance outlineAppearance;
	outlineAppearance.brush = outline;

	kke::EffectSourceAppearance accentAppearance;
	accentAppearance.brush = accent;

	kke::EffectSourceAppearance warmAppearance;
	warmAppearance.brush = warmAccent;

	engine().renderEffect(
		kke::Text{
			L"Space Grotesk via DWrite",
			{120.0f, 210.0f},
			{"Space Grotesk", 54.0f, kke::FontWeight::BOLD}
		},
		outlineAppearance,
		kke::ShadowEffect{{10.0f, 12.0f}, 10.0f, {0.0f, 0.0f, 0.0f, 0.78f}, kke::ShadowMode::ShadowWithSource});

	engine().renderEffect(
		kke::Text{
			L"MEDIUM weight with tighter blur",
			{120.0f, 310.0f},
			{"Space Grotesk", 32.0f, kke::FontWeight::MEDIUM}
		},
		accentAppearance,
		kke::ShadowEffect{{6.0f, 8.0f}, 5.0f, {0.02f, 0.06f, 0.14f, 0.74f}, kke::ShadowMode::ShadowWithSource});

	engine().renderEffect(
		kke::Text{
			L"Outline-like warm shadow",
			{120.0f, 392.0f},
			{"Space Grotesk", 40.0f, kke::FontWeight::BOLD}
		},
		warmAppearance,
		kke::ShadowEffect{{0.0f, 0.0f}, 7.0f, {0.30f, 0.08f, 0.02f, 0.78f}, kke::ShadowMode::ShadowWithSource});

	engine().renderEffect(
		kke::Text{
			L"Smaller labels also stay crisp",
			{120.0f, 470.0f},
			{"Space Grotesk", 24.0f, kke::FontWeight::MEDIUM}
		},
		outlineAppearance,
		kke::ShadowEffect{{4.0f, 5.0f}, 4.0f, {0.0f, 0.0f, 0.0f, 0.72f}, kke::ShadowMode::ShadowWithSource});
}
}
}
