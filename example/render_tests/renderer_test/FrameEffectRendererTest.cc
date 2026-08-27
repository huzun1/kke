#include "FrameEffectRendererTest.hh"

#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace application {
namespace renderer_test {
void FrameEffectRendererTest::render() {
	engine().draw(
	mountainTexture(),
	{{180.0f, 90.0f}, {1100.0f, 650.0f}},
	{0.95f, kke::TextureInterpolation::Linear}
);

kke::RoundedRect blurRect({{400.0f, 200.0f}, {600.0f, 400.0f}}, 20.0f);
engine().renderEffect(kke::BlurEffect(30.0f), blurRect);

kke::StrokeAppearance blurRectStrokeStyle{0.5f};
engine().renderEffect(
	blurRect,
	kke::EffectSourceAppearance{},
	kke::ShadowEffect{
		.color = {0.0f, 0.0f, 0.0f, 0.3f},
		.mode = kke::ShadowMode::OuterShadowOnly
	}
);
engine().draw(blurRect, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.6f}), blurRectStrokeStyle);
engine().fill(blurRect, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.2f}));

kke::Text title{
	L"KKE Renderer",
	{425.0f, 280.0f},
	{"Space Grotesk", 24.0f, kke::FontWeight::BOLD}
};
engine().renderEffect(
	title,
	kke::EffectSourceAppearance{},
	kke::ShadowEffect{
		.color = {0.0f, 0.0f, 0.0f, 0.3f},
		.mode = kke::ShadowMode::OuterShadowOnly
	}
);
engine().fill(title, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f}));
}
} // namespace renderer_test
} // namespace application
