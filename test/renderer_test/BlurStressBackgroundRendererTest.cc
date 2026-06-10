#include "BlurStressBackgroundRendererTest.hh"

#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/TextureInterpolation.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void BlurStressBackgroundRendererTest::render() {
	if (!mountainTexture()) {
		return;
	}

	engine().draw(
		mountainTexture(),
		{{0.0f, 0.0f}, {1920.0f, 1080.0f}},
		{1.0f, kke::TextureInterpolation::Linear}
	);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			kke::RoundedRect blurRect(
				{{i * 220.0f + 20.0f, j * 110.0f + 20.0f}, {i * 220.0f + 200.0f, j * 110.0f + 100.0f}},
				20.0f
			);
			engine().renderEffect(
				kke::BlurEffect({
					.radius = 30.0f,
					.optimization = kke::BlurOptimization::SPEED,
				}),
				blurRect
			);
		}
	}
}
} // namespace renderer_test
} // namespace application
