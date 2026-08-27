#include "ShapeEffectRendererTest.hh"

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/directional_blur/DirectionalBlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Polygon.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace application {
namespace renderer_test {
void ShapeEffectRendererTest::render() {
	kke::Brush background = kke::SolidColorBrush({0.74f, 0.79f, 0.85f, 1.0f});
	kke::Brush panelFill = kke::SolidColorBrush({0.88f, 0.91f, 0.95f, 1.0f});
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush accentFill = kke::SolidColorBrush({1.0f, 0.62f, 0.28f, 0.94f});
	kke::Brush coolFill = kke::SolidColorBrush({0.42f, 0.62f, 1.0f, 0.90f});
	kke::Brush outline = kke::SolidColorBrush({0.15f, 0.19f, 0.24f, 1.0f});

	engine().fill(kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}, background);
	engine().fill(kke::Rect{{50.0f, 56.0f}, {1230.0f, 664.0f}}, panelFill);

	kke::EffectSourceAppearance glowAppearance;
	glowAppearance.brush = normalLayerFill;
	glowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	kke::EffectSourceAppearance accentAppearance;
	accentAppearance.brush = accentFill;
	accentAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	kke::EffectSourceAppearance coolAppearance;
	coolAppearance.brush = coolFill;
	coolAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().renderEffect(
		kke::Ellipse{{320.0f, 240.0f}, 78.0f},
		glowAppearance,
		kke::BlurEffect{{18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}}
	);

	engine().fill(kke::Ellipse{{320.0f, 240.0f}, 78.0f}, normalLayerFill);

	engine().renderEffect(
		kke::Rect{{890.0f, 430.0f}, {1160.0f, 500.0f}},
		glowAppearance,
		kke::DirectionalBlurEffect{
			10.0f,
			18.0f,
			kke::BlurBorderMode::SOFT,
			kke::BlurOptimization::BALANCED
		}
	);

	engine().fill(kke::Rect{{890.0f, 430.0f}, {1160.0f, 500.0f}}, normalLayerFill);

	engine().renderEffect(
		kke::RoundedRect{kke::Rect{{120.0f, 420.0f}, {350.0f, 590.0f}}, 28.0f},
		accentAppearance,
		kke::ShadowEffect{
			{18.0f, 20.0f},
			14.0f,
			{0.0f, 0.0f, 0.0f, 0.56f},
			kke::ShadowMode::OuterShadowOnly
		}
	);

	engine().fill(
		kke::RoundedRect{kke::Rect{{120.0f, 420.0f}, {350.0f, 590.0f}}, 28.0f},
		accentFill
	);

	engine().renderEffect(
		kke::Ellipse{{700.0f, 220.0f}, 64.0f},
		glowAppearance,
		kke::BlurEffect{
			{14.0f,
			 kke::BlurBorderMode::SOFT,
			 kke::BlurOptimization::BALANCED,
			 kke::BlurMode::OuterOnly}
		},
		kke::Ellipse{{700.0f, 220.0f}, 64.0f}
	);

	engine().fill(
		kke::Ellipse{{700.0f, 220.0f}, 64.0f},
		kke::SolidColorBrush({0.93f, 0.95f, 0.98f, 1.0f})
	);

	engine().fill(kke::Triangle{{520.0f, 470.0f}, {600.0f, 610.0f}, {430.0f, 610.0f}}, coolFill);

	engine().renderEffect(
		kke::Triangle{{520.0f, 470.0f}, {600.0f, 610.0f}, {430.0f, 610.0f}},
		coolAppearance,
		kke::BlurEffect{
			{10.0f,
			 kke::BlurBorderMode::SOFT,
			 kke::BlurOptimization::BALANCED,
			 kke::BlurMode::OuterOnly}
		}
	);

	kke::Polygon polygon(
		{{900.0f, 170.0f},
		 {1030.0f, 150.0f},
		 {1100.0f, 250.0f},
		 {1040.0f, 350.0f},
		 {910.0f, 320.0f},
		 {860.0f, 230.0f}}
	);

	engine().renderEffect(
		kke::Geometry{polygon},
		coolAppearance,
		kke::ShadowEffect{
			{10.0f, 14.0f},
			10.0f,
			{0.0f, 0.0f, 0.0f, 0.52f},
			kke::ShadowMode::ShadowWithSource
		}
	);

	kke::EffectSourceAppearance panelShadowAppearance;
	panelShadowAppearance.brush = outline;
	panelShadowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().renderEffect(
		kke::Rect{{560.0f, 380.0f}, {820.0f, 600.0f}},
		panelShadowAppearance,
		kke::ShadowEffect{
			{14.0f, 18.0f},
			12.0f,
			{0.0f, 0.0f, 0.0f, 0.65f},
			kke::ShadowMode::OuterShadowOnly
		}
	);

	engine().fill(kke::Rect{{560.0f, 380.0f}, {820.0f, 600.0f}}, panelFill);

	engine().draw(kke::Geometry{polygon}, outline, {3.0f});
}
} // namespace renderer_test
} // namespace application
