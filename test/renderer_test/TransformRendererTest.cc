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
	kke::Brush background = kke::SolidColorBrush({0.85f, 0.89f, 0.94f, 1.0f});
	kke::Brush panelFill = kke::SolidColorBrush({0.15f, 0.19f, 0.26f, 0.97f});
	kke::Brush gridLine = kke::SolidColorBrush({0.22f, 0.28f, 0.36f, 0.22f});
	kke::Brush guideLine = kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 0.22f});
	kke::Brush referenceFill = kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 0.12f});
	kke::Brush shapeFill = kke::SolidColorBrush({0.33f, 0.78f, 0.94f, 0.95f});
	kke::Brush accentFill = kke::SolidColorBrush({0.96f, 0.55f, 0.32f, 0.95f});
	kke::Brush outline = kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 1.0f});

	engine().fill(kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}, background);
	engine().fill(kke::Rect{{46.0f, 72.0f}, {1234.0f, 644.0f}}, panelFill);
	engine().fill(kke::Rect{{398.0f, 96.0f}, {402.0f, 620.0f}}, gridLine);
	engine().fill(kke::Rect{{846.0f, 96.0f}, {850.0f, 620.0f}}, gridLine);

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

	engine().fill(kke::Rect{{80.0f, 140.0f}, {360.0f, 420.0f}}, panelFill);

	kke::RoundedRect scalingShape{
		kke::Rect{{164.0f, 236.0f}, {276.0f, 324.0f}},
		22.0f
	};

	kke::Point scalingCenter{220.0f, 280.0f};

	engine().fill(kke::Rect{{218.0f, 176.0f}, {222.0f, 384.0f}}, guideLine);
	engine().fill(kke::Rect{{116.0f, 278.0f}, {324.0f, 282.0f}}, guideLine);
	engine().fill(kke::Geometry{scalingShape}, referenceFill);
	engine().draw(kke::Geometry{scalingShape}, outline, {1.5f});

	engine().renderEffect(
		kke::Geometry{scalingShape},
		glowAppearance,
		kke::BlurEffect{{18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}});

	engine().pushTransform(kke::Scaling{scalingCenter, {1.55f, 1.30f}});
	engine().fill(kke::Geometry{scalingShape}, shapeFill);
	engine().draw(kke::Geometry{scalingShape}, outline, {3.0f});
	engine().popTransform();
	engine().fill(kke::Ellipse{scalingCenter, 4.0f}, outline);

	engine().fill(kke::Text{
		L"Rotate",
		{504.0f, 116.0f},
		{"Space Grotesk", 22.0f, kke::FontWeight::BOLD}
	}, outline);

	engine().fill(kke::Rect{{440.0f, 140.0f}, {840.0f, 420.0f}}, panelFill);

	engine().renderEffect(
		kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}},
		shadowAppearance,
		kke::ShadowEffect{{14.0f, 16.0f}, 12.0f, {0.0f, 0.0f, 0.0f, 0.42f}, kke::ShadowMode::OuterShadowOnly});

	engine().pushTransform(kke::Rotation{{640.0f, 255.0f}, 28.0f});
	engine().fill(kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}}, accentFill);
	engine().draw(kke::Triangle{{640.0f, 180.0f}, {730.0f, 330.0f}, {550.0f, 330.0f}}, outline, {3.0f});
	engine().popTransform();

	engine().fill(kke::Text{
		L"Transform Stack",
		{904.0f, 116.0f},
		{"Space Grotesk", 22.0f, kke::FontWeight::BOLD}
	}, outline);

	engine().fill(kke::Rect{{880.0f, 140.0f}, {1200.0f, 520.0f}}, panelFill);

	engine().pushTransform(kke::Translation{{48.0f, 44.0f}});
	engine().pushTransform(kke::Scaling{{1010.0f, 300.0f}, {1.20f, 0.85f}});
	engine().pushTransform(kke::Rotation{{1010.0f, 300.0f}, -18.0f});

	engine().renderEffect(
		kke::Ellipse{{1010.0f, 300.0f}, 72.0f},
		glowAppearance,
		kke::BlurEffect{{16.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::OuterOnly}});

	engine().fill(kke::Ellipse{{1010.0f, 300.0f}, 72.0f}, shapeFill);
	engine().draw(kke::Rect{{950.0f, 250.0f}, {1070.0f, 350.0f}}, outline, {3.0f});

	engine().popTransform();
	engine().popTransform();
	engine().popTransform();

	engine().fill(kke::Ellipse{{1010.0f, 300.0f}, 4.0f}, outline);
}
}
}
