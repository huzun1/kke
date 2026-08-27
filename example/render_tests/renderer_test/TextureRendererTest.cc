#include "TextureRendererTest.hh"

#include "kke/appearance/resource/Canvas.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/color_matrix/ColorMatrixEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/TextureInterpolation.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void TextureRendererTest::render() {
	kke::Brush panelFill = kke::SolidColorBrush({0.12f, 0.16f, 0.22f, 1.0f});
	engine().fill(kke::Rect{{80.0f, 80.0f}, {1220.0f, 640.0f}}, panelFill);

	if (encodedTexture()) {
		engine().draw(
			encodedTexture(),
			{{560.0f, 110.0f}, {860.0f, 320.0f}},
			{0.95f, kke::TextureInterpolation::Linear, kke::Rect{{90.0f, 40.0f}, {360.0f, 220.0f}}}
		);
	}

	if (!rawTexture()) {
		return;
	}

	engine().draw(
		rawTexture(),
		{{900.0f, 110.0f}, {1160.0f, 370.0f}},
		{0.90f, kke::TextureInterpolation::Nearest}
	);

	kke::ColorMatrixEffect colorMatrixEffect;
	colorMatrixEffect.colorMatrix.values = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	colorMatrixEffect.clampOutput = true;

	engine().renderEffect(colorMatrixEffect, kke::Rect{{900.0f, 110.0f}, {1160.0f, 370.0f}});

	std::shared_ptr<kke::Canvas> effectCanvas = engine().createCanvas();
	engine().beginCanvas(effectCanvas);

	if (encodedTexture()) {
		engine().draw(
			encodedTexture(),
			{{120.0f, 420.0f}, {420.0f, 610.0f}},
			{0.98f, kke::TextureInterpolation::Linear, kke::Rect{{40.0f, 20.0f}, {330.0f, 220.0f}}}
		);
	}

	engine().draw(
		rawTexture(),
		{{300.0f, 460.0f}, {520.0f, 620.0f}},
		{0.88f, kke::TextureInterpolation::Nearest}
	);

	engine().endCanvas();
	engine().finishCanvas(effectCanvas);

	kke::EffectSourceAppearance canvasAppearance;

	engine().renderEffect(
		effectCanvas,
		canvasAppearance,
		kke::ShadowEffect{
			{20.0f, 18.0f},
			16.0f,
			{0.0f, 0.0f, 0.0f, 0.50f},
			kke::ShadowMode::OuterShadowOnly
		}
	);

	engine().draw(effectCanvas, 1.0f);

	engine().renderEffect(
		effectCanvas,
		canvasAppearance,
		kke::BlurEffect{{10.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}},
		kke::Rect{{620.0f, 420.0f}, {1060.0f, 640.0f}}
	);

	engine().pushTransform(kke::Translation{{500.0f, 0.0f}});
	engine().draw(effectCanvas, 0.96f);
	engine().popTransform();
}
} // namespace renderer_test
} // namespace application
