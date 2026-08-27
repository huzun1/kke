#include "TransformPage.hh"

#include <algorithm>
#include <cmath>

#include <Windows.h>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/d2d/D2dEngine.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke::example::overlay {
TransformPage::TransformPage() : animationStartTime(GetTickCount64()) {
}

void TransformPage::render(kke::D2dEngine& engine, kke::Scale viewportSize) const {
	float left = (std::max)(470.0f, viewportSize.x * 0.38f);
	float top = (std::max)(150.0f, viewportSize.y * 0.24f);
	float elapsedSeconds = static_cast<float>(GetTickCount64() - animationStartTime) * 0.001f;

	kke::Brush referenceBrush = kke::SolidColorBrush({0.94f, 0.97f, 1.0f, 0.25f});
	kke::Brush rotateBrush = kke::SolidColorBrush({0.95f, 0.36f, 0.42f, 0.95f});
	kke::Brush scaleBrush = kke::SolidColorBrush({0.28f, 0.78f, 1.0f, 0.95f});
	kke::Brush translateBrush = kke::SolidColorBrush({0.25f, 0.90f, 0.62f, 0.95f});

	kke::Triangle rotatingTriangle{
		{left + 90.0f, top},
		{left + 165.0f, top + 130.0f},
		{left + 15.0f, top + 130.0f}
	};
	engine.draw(rotatingTriangle, referenceBrush, {3.0f});
	engine.pushTransform(kke::Rotation{{left + 90.0f, top + 78.0f}, elapsedSeconds * 48.0f});
	engine.fill(rotatingTriangle, rotateBrush);
	engine.popTransform();

	kke::RoundedRect scalingRect{
		{{left + 245.0f, top + 25.0f}, {left + 365.0f, top + 125.0f}},
		18.0f
	};
	engine.draw(kke::Geometry{scalingRect}, referenceBrush, {3.0f});
	float scale = 1.0f + std::sin(elapsedSeconds * 2.0f) * 0.35f;
	engine.pushTransform(kke::Scaling{{left + 305.0f, top + 75.0f}, {scale, scale}});
	engine.fill(kke::Geometry{scalingRect}, scaleBrush);
	engine.popTransform();

	kke::Ellipse translatingEllipse{{left + 505.0f, top + 75.0f}, 54.0f};
	engine.draw(translatingEllipse, referenceBrush, {3.0f});
	engine.pushTransform(kke::Translation{{std::sin(elapsedSeconds * 1.7f) * 80.0f, 70.0f}});
	engine.fill(translatingEllipse, translateBrush);
	engine.popTransform();

	kke::Brush textBrush = kke::SolidColorBrush({0.96f, 0.98f, 1.0f, 1.0f});
	engine.fill(
		kke::Text{L"Rotate", {left + 48.0f, top + 170.0f}, {"Segoe UI", 20.0f, kke::FontWeight::BOLD}},
		textBrush
	);
	engine.fill(
		kke::Text{L"Scaling", {left + 267.0f, top + 170.0f}, {"Segoe UI", 20.0f, kke::FontWeight::BOLD}},
		textBrush
	);
	engine.fill(
		kke::Text{L"Translate", {left + 463.0f, top + 240.0f}, {"Segoe UI", 20.0f, kke::FontWeight::BOLD}},
		textBrush
	);
}
} // namespace kke::example::overlay
