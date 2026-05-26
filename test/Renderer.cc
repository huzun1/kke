#include "Renderer.hh"

#include <cstdio>
#include <memory>

#include <Windows.h>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "resources/resources.h"

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	auto [fontData, fontDataSize] = loadResource(FONT_SPACE_GROTESK);
	std::shared_ptr<kke::Font> font = engine.uploadFont(fontData, fontDataSize);
	if (font) {
		std::printf("font uploaded: %p, %zu\n", fontData, fontDataSize);
	}
}

void application::Renderer::render() {
	ID2D1DeviceContext* deviceContext = d2d1.getDeviceContext();
	ID2D1Bitmap1* renderTarget = d2d1.getRenderTarget();
	if (!deviceContext || !renderTarget) {
		return;
	}

	kke::D2dContext context(d2d1.getFactory(), deviceContext);

	engine.beginDraw(context, renderTarget);
	engine.clear();
	renderFrame();
	engine.endDraw();
}

void application::Renderer::renderFrame() {
	kke::Brush background = kke::SolidColorBrush({0.08f, 0.10f, 0.12f, 1.0f});
	kke::Brush normalLayerFill = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});
	kke::Brush invertedLayerFill = kke::SolidColorBrush({0.30f, 0.52f, 1.0f, 0.55f});
	kke::Brush outline = kke::SolidColorBrush({0.92f, 0.94f, 0.96f, 1.0f});

	kke::Geometry fullViewport = kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}};
	kke::Geometry normalMask = kke::Rect{{120.0f, 110.0f}, {520.0f, 430.0f}};
	kke::Geometry invertedMask = kke::Ellipse{{850.0f, 280.0f}, 145.0f};

	engine.fill(fullViewport, background);

	engine.pushLayer(normalMask);
	engine.fill(fullViewport, normalLayerFill);
	engine.popLayer();
	engine.draw(normalMask, outline, {4.0f});

	engine.pushLayer(invertedMask, kke::LayerMode::Inverted);
	engine.fill(fullViewport, invertedLayerFill);
	engine.popLayer();
	engine.draw(invertedMask, outline, {4.0f});

	kke::Text label{
		L"Space Grotesk via DWrite",
		{120.0f, 500.0f},
		{"Space Grotesk", 36.0f, kke::FontWeight::BOLD}
	};
	engine.fill(label, outline);

	fpsCounter.frame();
}

std::pair<void const*, size_t> application::Renderer::loadResource(int resourceId) {
	const HRSRC resourceInfo = FindResourceA(nullptr, MAKEINTRESOURCEA(resourceId), "WAVE");
	if (!resourceInfo) {
		return {nullptr, 0};
	}

	const HGLOBAL resourceData = LoadResource(nullptr, resourceInfo);
	if (!resourceData) {
		return {nullptr, 0};
	}

	const void* resourcePtr = LockResource(resourceData);
	const DWORD resourceSize = SizeofResource(nullptr, resourceInfo);
	return {resourcePtr, static_cast<size_t>(resourceSize)};
}
