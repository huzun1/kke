#include "BlurStressRendererTest.hh"

#include <cstdio>
#include <chrono>

#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void BlurStressRendererTest::render() {
	kke::Brush fillBrush = kke::SolidColorBrush({0.12f, 0.78f, 0.45f, 0.85f});

	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			kke::RoundedRect blurRect(
				{{i * 400.0f, j * 400.0f}, {i * 400.0f + 300.0f, j * 400.0f + 300.0f}},
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
