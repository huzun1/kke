#include "FrameEffectRendererTest.hh"

#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void FrameEffectRendererTest::render() {
	engine().fill(
		kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}},
		kke::SolidColorBrush({0.78f, 0.83f, 0.88f, 1.0f})
	);

	engine().fill(
		kke::Rect{{120.0f, 110.0f}, {470.0f, 360.0f}},
		kke::SolidColorBrush({0.16f, 0.22f, 0.30f, 0.95f})
	);

	engine().fill(
		kke::Rect{{220.0f, 180.0f}, {540.0f, 410.0f}},
		kke::SolidColorBrush({0.20f, 0.76f, 0.44f, 0.90f})
	);

	engine().fill(
		kke::Rect{{430.0f, 130.0f}, {780.0f, 340.0f}},
		kke::SolidColorBrush({0.96f, 0.58f, 0.28f, 0.82f})
	);

	engine().fill(
		kke::Rect{{680.0f, 230.0f}, {1100.0f, 560.0f}},
		kke::SolidColorBrush({0.32f, 0.56f, 0.98f, 0.78f})
	);

	engine().fill(
		kke::Rect{{320.0f, 420.0f}, {960.0f, 610.0f}},
		kke::SolidColorBrush({0.95f, 0.96f, 0.98f, 0.88f})
	);

	kke::EffectCompose frameEffects;
	frameEffects.addEffect(kke::ShadowEffect{
		{0.0f, 18.0f},
		24.0f,
		{0.02f, 0.04f, 0.08f, 0.22f},
		kke::ShadowMode::ShadowWithSource
	});
	frameEffects.addEffect(kke::BlurEffect{
		{4.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}
	});
	engine().renderEffect(frameEffects, kke::Rect{{110.0f, 100.0f}, {1130.0f, 620.0f}});
}
} // namespace renderer_test
} // namespace application
