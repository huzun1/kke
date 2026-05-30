#include "TextureRendererTest.hh"

#include "kke/appearance/resource/effect/color_matrix/ColorMatrixEffect.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/TextureInterpolation.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void TextureRendererTest::render() {
	if (encodedTexture()) {
		engine().draw(
			encodedTexture(),
			{{560.0f, 110.0f}, {860.0f, 320.0f}},
			{0.95f, kke::TextureInterpolation::Linear, kke::Rect{{90.0f, 40.0f}, {360.0f, 220.0f}}});
	}

	if (!rawTexture()) {
		return;
	}

	engine().draw(
		rawTexture(),
		{{900.0f, 110.0f}, {1160.0f, 370.0f}},
		{0.90f, kke::TextureInterpolation::Nearest});

	kke::ColorMatrixEffect colorMatrixEffect;
	colorMatrixEffect.colorMatrix.values = {
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	colorMatrixEffect.clampOutput = true;

	engine().renderEffect(colorMatrixEffect, kke::Rect{{900.0f, 110.0f}, {1160.0f, 370.0f}});
}
}
}
