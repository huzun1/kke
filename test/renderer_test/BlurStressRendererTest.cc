#include "BlurStressRendererTest.hh"

#include <cstdio>

#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void BlurStressRendererTest::render() {
	static bool announced = false;
	if (!announced) {
		std::printf("blur stress benchmark: 300 blurs per frame\n");
		announced = true;
	}

	kke::Brush panelFill = kke::SolidColorBrush({0.14f, 0.18f, 0.22f, 1.0f});
	kke::Brush blurMask = kke::SolidColorBrush({0.94f, 0.96f, 0.98f, 1.0f});

	kke::EffectSourceAppearance blurAppearance;
	blurAppearance.brush = blurMask;
	blurAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	kke::BlurEffect blurEffect{
		{10.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::OuterOnly
		}
	};

	constexpr int columns = 20;
	constexpr int rows = 15;
	constexpr float width = 44.0f;
	constexpr float height = 28.0f;
	constexpr float gapX = 14.0f;
	constexpr float gapY = 14.0f;
	constexpr float originX = 32.0f;
	constexpr float originY = 28.0f;
	constexpr float rounding = 8.0f;

	for (int row = 0; row < rows; ++row) {
		for (int column = 0; column < columns; ++column) {
			float left = originX + column * (width + gapX);
			float top = originY + row * (height + gapY);
			kke::RoundedRect panel{kke::Rect{{left, top}, {left + width, top + height}}, rounding};

			engine().renderEffect(kke::Geometry{panel}, blurAppearance, blurEffect);
		}
	}
}
} // namespace renderer_test
} // namespace application
