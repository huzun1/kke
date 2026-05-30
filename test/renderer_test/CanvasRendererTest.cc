#include "CanvasRendererTest.hh"

#include <memory>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/Sources.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void CanvasRendererTest::render() {
	kke::Brush background = kke::SolidColorBrush({0.83f, 0.87f, 0.92f, 1.0f});
	kke::Brush panelFill = kke::SolidColorBrush({0.18f, 0.22f, 0.29f, 0.96f});
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush accentFill = kke::SolidColorBrush({0.98f, 0.62f, 0.28f, 0.92f});
	kke::Brush coolFill = kke::SolidColorBrush({0.32f, 0.58f, 1.0f, 0.86f});
	kke::Brush outline = kke::SolidColorBrush({0.94f, 0.96f, 0.98f, 1.0f});

	engine().fill(kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}, background);
	engine().fill(kke::Rect{{56.0f, 76.0f}, {1224.0f, 656.0f}}, panelFill);

	std::shared_ptr<kke::Canvas> canvas = engine().createCanvas();
	engine().pushCanvas(canvas);

	engine().fill(
		kke::RoundedRect{kke::Rect{{920.0f, 420.0f}, {1140.0f, 610.0f}}, 24.0f},
		normalLayerFill);

	engine().fill(
		kke::Rect{{955.0f, 455.0f}, {1105.0f, 520.0f}},
		accentFill);

	engine().fill(
		kke::Ellipse{{1030.0f, 540.0f}, 54.0f},
		coolFill);

	engine().draw(kke::Ellipse{{1030.0f, 540.0f}, 54.0f}, outline, {4.0f});
	engine().draw(kke::RoundedRect{kke::Rect{{920.0f, 420.0f}, {1140.0f, 610.0f}}, 24.0f}, outline, {3.0f});
	engine().popCanvas();

	kke::EffectSourceAppearance shadowAppearance;

	engine().renderEffect(
		canvas,
		shadowAppearance,
		kke::ShadowEffect{{16.0f, 18.0f}, 16.0f, {0.0f, 0.0f, 0.0f, 0.28f}, kke::ShadowMode::OuterShadowOnly});

	engine().pushTransform(kke::Translation({-460.0f, -220.0f}));
	engine().draw(canvas);
	engine().popTransform();

	kke::EffectSourceAppearance blurAppearance;
	blurAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	engine().renderEffect(
		canvas,
		blurAppearance,
		kke::BlurEffect{{12.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}});

	engine().pushTransform(kke::Translation({-210.0f, -40.0f}));
	engine().draw(canvas, 0.95f);
	engine().popTransform();
}
}
}
