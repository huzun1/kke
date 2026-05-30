#include "TransformRendererTest.hh"

#include "kke/appearance/Text.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace application {
namespace renderer_test {
void TransformRendererTest::render() {
	kke::Brush background = kke::SolidColorBrush({0.08f, 0.10f, 0.12f, 1.0f});
	kke::Brush panelFill = kke::SolidColorBrush({0.14f, 0.18f, 0.24f, 1.0f});
	kke::Brush shapeFill = kke::SolidColorBrush({0.33f, 0.78f, 0.94f, 0.95f});
	kke::Brush accentFill = kke::SolidColorBrush({0.96f, 0.55f, 0.32f, 0.95f});
	kke::Brush outline = kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 1.0f});

	engine().fill(kke::Geometry{kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}}, background);

	kke::EffectSourceAppearance glowAppearance;
	glowAppearance.brush = shapeFill;
	glowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	kke::EffectSourceAppearance shadowAppearance;
	shadowAppearance.brush = accentFill;
	shadowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().fill(kke::Text{
		L"Scaling",
		{124.0f, 116.0f},
		{"Space Grotesk", 22.0f, kke::FontWeight::BOLD}
	}, outline);

	engine().fill(kke::Geometry{kke::Rect{{80.0f, 140.0f}, {360.0f, 420.0f}}}, panelFill);

	engine().renderEffect(
		kke::Geometry{kke::RoundedRect{kke::Rect{{120.0f, 190.0f}, {220.0f, 270.0f}}, 20.0f}},
		glowAppearance,
		kke::BlurEffect{{20.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}});

	engine().pushTransform(kke::Scaling{{170.0f, 230.0f}, {1.9f, 1.35f}});
	engine().fill(kke::Geometry{kke::RoundedRect{kke::Rect{{120.0f, 190.0f}, {220.0f, 270.0f}}, 20.0f}}, shapeFill);
	engine().draw(kke::Geometry{kke::RoundedRect{kke::Rect{{120.0f, 190.0f}, {220.0f, 270.0f}}, 20.0f}}, outline, {3.0f});
	engine().popTransform();

	engine().fill(kke::Text{
		L"Rotate",
		{504.0f, 116.0f},
		{"Space Grotesk", 22.0f, kke::FontWeight::BOLD}
	}, outline);

	engine().fill(kke::Geometry{kke::Rect{{440.0f, 140.0f}, {840.0f, 420.0f}}}, panelFill);

	engine().renderEffect(
		kke::Geometry{kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}}},
		shadowAppearance,
		kke::ShadowEffect{{14.0f, 16.0f}, 12.0f, {0.0f, 0.0f, 0.0f, 0.55f}, kke::ShadowMode::OuterShadowOnly});

	engine().pushTransform(kke::Rotation{{640.0f, 255.0f}, 28.0f});
	engine().fill(kke::Geometry{kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}}}, accentFill);
	engine().draw(kke::Geometry{kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}}}, outline, {3.0f});
	engine().popTransform();

	engine().fill(kke::Text{
		L"Transform Stack",
		{904.0f, 116.0f},
		{"Space Grotesk", 22.0f, kke::FontWeight::BOLD}
	}, outline);

	engine().fill(kke::Geometry{kke::Rect{{880.0f, 140.0f}, {1200.0f, 520.0f}}}, panelFill);

	engine().pushTransform(kke::Translation{{48.0f, 44.0f}});
	engine().pushTransform(kke::Scaling{{1010.0f, 300.0f}, {1.20f, 0.85f}});
	engine().pushTransform(kke::Rotation{{1010.0f, 300.0f}, -18.0f});

	engine().renderEffect(
		kke::Geometry{kke::Ellipse{{1010.0f, 300.0f}, 72.0f}},
		glowAppearance,
		kke::BlurEffect{{16.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::OuterOnly}});

	engine().fill(kke::Geometry{kke::Ellipse{{1010.0f, 300.0f}, 72.0f}}, shapeFill);
	engine().draw(kke::Geometry{kke::Rect{{950.0f, 250.0f}, {1070.0f, 350.0f}}}, outline, {3.0f});

	engine().popTransform();
	engine().popTransform();
	engine().popTransform();
}
}
}
