#include "FillPage.hh"

#include <algorithm>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/LinearGradientBrush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowMode.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"

namespace kke::example::overlay {
void FillPage::render(kke::D2dEngine& engine, kke::Scale viewportSize) {
	float left = (std::max)(480.0f, viewportSize.x * 0.42f);
	float top = (std::max)(120.0f, viewportSize.y * 0.20f);

	kke::Brush fillBrush = kke::SolidColorBrush({0.18f, 0.68f, 0.96f, 0.96f});
	kke::Brush borderBrush = kke::SolidColorBrush({0.94f, 0.97f, 1.0f, 1.0f});
	kke::Brush gradientBrush = kke::LinearGradientBrush(
		{{0.55f, 0.28f, 1.0f, 1.0f}, {0.10f, 0.88f, 0.72f, 1.0f}},
		{left + 36.0f, top + 180.0f},
		{left + 356.0f, top + 180.0f}
	);

	engine.fill(kke::Ellipse{{left + 105.0f, top + 112.0f}, 54.0f}, fillBrush);
	engine.draw(kke::Ellipse{{left + 105.0f, top + 112.0f}, 54.0f}, borderBrush, {5.0f});
	engine.fill(
		kke::RoundedRect{{{left + 36.0f, top + 180.0f}, {left + 394.0f, top + 252.0f}}, 18.0f},
		gradientBrush
	);

	kke::EffectSourceAppearance textAppearance;
	textAppearance.brush = borderBrush;
	engine.renderEffect(
		kke::Text{
			L"Fill, border, gradient, and text",
			{left + 36.0f, top + 276.0f},
			{"Segoe UI", 22.0f, kke::FontWeight::SEMI_BOLD}
		},
		textAppearance,
		kke::ShadowEffect{
			{3.0f, 4.0f},
			4.0f,
			{0.0f, 0.0f, 0.0f, 0.82f},
			kke::ShadowMode::ShadowWithSource
		}
	);
}
} // namespace kke::example::overlay
