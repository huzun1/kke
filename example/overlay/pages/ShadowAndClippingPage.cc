#include "ShadowAndClippingPage.hh"

#include <algorithm>
#include <cmath>

#include <Windows.h>

#include "kke/appearance/resource/brush/impl/LinearGradientBrush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/blur/BlurEffect.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke::example::overlay {
ShadowAndClippingPage::ShadowAndClippingPage() : animationStartTime(GetTickCount64()) {
}

void ShadowAndClippingPage::render(kke::D2dEngine& engine, kke::Scale viewportSize) const {
	float left = (std::max)(480.0f, viewportSize.x * 0.42f);
	float top = (std::max)(145.0f, viewportSize.y * 0.22f);
	float elapsedSeconds = static_cast<float>(GetTickCount64() - animationStartTime) * 0.001f;
	float gradientAngle = elapsedSeconds * 0.8f;
	float gradientDirectionX = std::cos(gradientAngle) * 230.0f;
	float gradientDirectionY = std::sin(gradientAngle) * 230.0f;
	float centerX = left + 190.0f;
	float centerY = top + 125.0f;

	kke::RoundedRect card{{{left, top}, {left + 380.0f, top + 250.0f}}, 34.0f};
	kke::LinearGradientBrush cardGradient(
		{{0.14f, 0.36f, 0.94f, 1.0f},
		 {0.55f, 0.24f, 0.94f, 1.0f},
		 {0.05f, 0.82f, 0.72f, 1.0f}},
		{centerX - gradientDirectionX, centerY - gradientDirectionY},
		{centerX + gradientDirectionX, centerY + gradientDirectionY}
	);
	kke::EffectSourceAppearance shadowAppearance;
	shadowAppearance.brush = kke::LinearGradientBrush(
		{{0.14f, 0.36f, 0.94f, 0.72f},
		 {0.55f, 0.24f, 0.94f, 0.72f},
		 {0.05f, 0.82f, 0.72f, 0.72f}},
		{centerX - gradientDirectionX, centerY - gradientDirectionY},
		{centerX + gradientDirectionX, centerY + gradientDirectionY}
	);
	engine.pushTransform(kke::Translation{{22.0f, 26.0f}});
	engine.renderEffect(
		kke::Geometry{card},
		shadowAppearance,
		kke::BlurEffect(16.0f)
	);
	engine.popTransform();
	engine.fill(kke::Geometry{card}, cardGradient);

	engine.pushLayer(kke::Geometry{card});
	engine.fill(
		kke::Ellipse{{left + 330.0f, top + 20.0f}, 105.0f},
		kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.22f})
	);
	engine.fill(
		kke::Rect{{left - 35.0f, top + 170.0f}, {left + 430.0f, top + 205.0f}},
		kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.24f})
	);
	engine.popLayer();
	engine.draw(
		kke::Geometry{card},
		kke::SolidColorBrush({0.95f, 0.98f, 1.0f, 0.95f}),
		{3.0f}
	);
}
} // namespace kke::example::overlay
