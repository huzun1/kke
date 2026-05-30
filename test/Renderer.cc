#include "Renderer.hh"

#include <cstdio>
#include <cstdint>
#include <memory>
#include <vector>

#include <Windows.h>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/TextureInterpolation.hh"
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
	ensureTexturesUploaded();
	renderFrame();
	engine.endDraw();
}

void application::Renderer::ensureTexturesUploaded() {
	if (!encodedTexture) {
		auto [textureData, textureDataSize] = loadResource(TEXTURE_DYCONTRAST);
		encodedTexture = engine.uploadTexture(textureData, textureDataSize);
	}

	if (!rawTexture) {
		constexpr uint32_t rawTextureWidth = 8;
		constexpr uint32_t rawTextureHeight = 8;

		if (rawTexturePixels.empty()) {
			rawTexturePixels = createRawTexturePixels(rawTextureWidth, rawTextureHeight);
		}

		rawTexture = engine.uploadTexture({
			rawTexturePixels.data(),
			rawTextureWidth,
			rawTextureHeight,
			rawTextureWidth * 4
		});
	}
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

	std::shared_ptr<kke::Canvas> canvas = engine.createCanvas();
	engine.pushCanvas(canvas);
	engine.fill(kke::Geometry{kke::Rect{{930.0f, 440.0f}, {1110.0f, 600.0f}}}, normalLayerFill);
	engine.draw(kke::Geometry{kke::Ellipse{{1020.0f, 520.0f}, 58.0f}}, outline, {4.0f});
	engine.popCanvas();
	engine.draw(canvas, 0.85f);

	if (encodedTexture) {
		engine.draw(
			encodedTexture,
			{{560.0f, 110.0f}, {860.0f, 320.0f}},
			{0.95f, kke::TextureInterpolation::Linear, kke::Rect{{90.0f, 40.0f}, {360.0f, 220.0f}}});
	}

	if (rawTexture) {
		engine.draw(
			rawTexture,
			{{900.0f, 110.0f}, {1160.0f, 370.0f}},
			{0.90f, kke::TextureInterpolation::Nearest});
	}

	kke::Text label{
		L"Space Grotesk via DWrite",
		{120.0f, 500.0f},
		{"Space Grotesk", 36.0f, kke::FontWeight::BOLD}
	};
	engine.fill(label, outline);

	fpsCounter.frame();
}

std::vector<uint8_t> application::Renderer::createRawTexturePixels(uint32_t width, uint32_t height) {
	std::vector<uint8_t> pixels(static_cast<size_t>(width) * height * 4);

	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			size_t pixelIndex = (static_cast<size_t>(y) * width + x) * 4;
			bool isBrightCell = ((x / 2) + (y / 2)) % 2 == 0;

			pixels[pixelIndex + 0] = isBrightCell ? 0xFF : 0x1F;
			pixels[pixelIndex + 1] = isBrightCell ? 0xB3 : 0x92;
			pixels[pixelIndex + 2] = isBrightCell ? 0x2D : 0xFF;
			pixels[pixelIndex + 3] = (x == y || x + y == width - 1) ? 0xA8 : 0xFF;
		}
	}

	return pixels;
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
