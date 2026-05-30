#include "ShadowStressRendererTest.hh"

#include <cstdio>

#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void ShadowStressRendererTest::render() {
	static bool announced = false;
	if (!announced) {
		std::printf("shadow stress benchmark: 300 shadows per frame\n");
		announced = true;
	}

	kke::Brush panelFill = kke::SolidColorBrush({0.14f, 0.18f, 0.22f, 1.0f});
	kke::Brush shadowMask = kke::SolidColorBrush({0.94f, 0.96f, 0.98f, 1.0f});

	kke::EffectSourceAppearance shadowAppearance;
	shadowAppearance.brush = shadowMask;
	shadowAppearance.drawMode = kke::EffectSourceDrawMode::Fill;

	kke::ShadowEffect shadowEffect{
		{10.0f, 12.0f},
		10.0f,
		{0.0f, 0.0f, 0.0f, 0.55f},
		kke::ShadowMode::OuterShadowOnly
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
			kke::RoundedRect panel{
				kke::Rect{{left, top}, {left + width, top + height}},
				rounding
			};

			engine().renderEffect(
				kke::Geometry{panel},
				shadowAppearance,
				shadowEffect);

			engine().fill(kke::Geometry{panel}, panelFill);
		}
	}
}
}
}
