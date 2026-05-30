#include "ShapeEffectRendererTest.hh"

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/directional_blur/DirectionalBlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void ShapeEffectRendererTest::render() {
	kke::Brush background = kke::SolidColorBrush({0.08f, 0.10f, 0.12f, 1.0f});
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush outline = kke::SolidColorBrush({0.92f, 0.94f, 0.96f, 1.0f});

	kke::EffectSourceAppearance glowAppearance;
	glowAppearance.brush = normalLayerFill;
	glowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().renderEffect(
		kke::Geometry{kke::Ellipse{{320.0f, 240.0f}, 78.0f}},
		glowAppearance,
		kke::BlurEffect{{18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}});

	engine().renderEffect(
		kke::Geometry{kke::Rect{{890.0f, 430.0f}, {1160.0f, 500.0f}}},
		glowAppearance,
		kke::DirectionalBlurEffect{10.0f, 18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED});

	engine().renderEffect(
		kke::Geometry{kke::Ellipse{{700.0f, 220.0f}, 64.0f}},
		glowAppearance,
		kke::BlurEffect{{14.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::OuterOnly}},
		kke::Ellipse{{700.0f, 220.0f}, 64.0f});

	kke::EffectSourceAppearance panelShadowAppearance;
	panelShadowAppearance.brush = outline;
	panelShadowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().renderEffect(
		kke::Geometry{kke::Rect{{560.0f, 380.0f}, {820.0f, 600.0f}}},
		panelShadowAppearance,
		kke::ShadowEffect{{14.0f, 18.0f}, 12.0f, {0.0f, 0.0f, 0.0f, 0.65f}, kke::ShadowMode::OuterShadowOnly});

	engine().fill(
		kke::Geometry{kke::Rect{{560.0f, 380.0f}, {820.0f, 600.0f}}},
		background);
}
}
}
