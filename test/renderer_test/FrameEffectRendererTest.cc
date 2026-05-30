#include "FrameEffectRendererTest.hh"

#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void FrameEffectRendererTest::render() {
	kke::EffectCompose frameEffects;
	frameEffects.addEffect(kke::BlurEffect{{1.5f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}});
	engine().renderEffect(frameEffects, kke::Rect{{0.0f, 0.0f}, {520.0f, 430.0f}});
}
}
}
