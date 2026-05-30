#include "CanvasRendererTest.hh"

#include <memory>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/engine/Sources.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void CanvasRendererTest::render() {
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush outline = kke::SolidColorBrush({0.92f, 0.94f, 0.96f, 1.0f});

	std::shared_ptr<kke::Canvas> canvas = engine().createCanvas();
	engine().pushCanvas(canvas);
	engine().fill(kke::Geometry{kke::Rect{{930.0f, 440.0f}, {1110.0f, 600.0f}}}, normalLayerFill);
	engine().draw(kke::Geometry{kke::Ellipse{{1020.0f, 520.0f}, 58.0f}}, outline, {4.0f});
	engine().popCanvas();

	engine().pushTransform(kke::Translation({-100.0f, -100.0f}));
	engine().draw(canvas);
	engine().popTransform();

	kke::EffectSourceAppearance blurAppearance;
	kke::BlurEffect blurEffect{
		{10.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}
	};

	engine().renderEffect(canvas, blurAppearance, blurEffect);
}
}
}
