#include "FullBlurPage.hh"

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke::example::overlay {
void FullBlurPage::render(kke::D2dEngine& engine, kke::Scale viewportSize) {
	engine.renderEffect(
		kke::BlurEffect{{
			.radius = 4.0f,
			.borderMode = kke::BlurBorderMode::HARD,
			.optimization = kke::BlurOptimization::BALANCED,
		}},
		kke::Rect{{0.0f, 0.0f}, {viewportSize.x, viewportSize.y}},
		1.0f
	);

	kke::RoundedRect label{
		{{viewportSize.x * 0.5f - 170.0f, viewportSize.y * 0.5f - 42.0f},
		 {viewportSize.x * 0.5f + 170.0f, viewportSize.y * 0.5f + 42.0f}},
		22.0f
	};
	engine.fill(kke::Geometry{label}, kke::SolidColorBrush({0.04f, 0.06f, 0.10f, 0.82f}));
	engine.fill(
		kke::Text{
			L"Full-frame blur",
			{viewportSize.x * 0.5f - 91.0f, viewportSize.y * 0.5f - 16.0f},
			{"Segoe UI", 25.0f, kke::FontWeight::BOLD}
		},
		kke::SolidColorBrush({0.96f, 0.98f, 1.0f, 1.0f})
	);
}
} // namespace kke::example::overlay
