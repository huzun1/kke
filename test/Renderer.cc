#include "Renderer.hh"

#include <cstdio>
#include <cstdint>
#include <memory>
#include <vector>

#include <Windows.h>

#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "renderer_test/CanvasRendererTest.hh"
#include "renderer_test/FrameEffectRendererTest.hh"
#include "renderer_test/LayerRendererTest.hh"
#include "renderer_test/ShapeEffectRendererTest.hh"
#include "renderer_test/TextEffectRendererTest.hh"
#include "renderer_test/TextureRendererTest.hh"
#include "resources/resources.h"

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	auto [fontData, fontDataSize] = loadResource(FONT_SPACE_GROTESK);
	std::shared_ptr<kke::Font> font = engine.uploadFont(fontData, fontDataSize);
	if (font) {
		std::printf("font uploaded: %p, %zu\n", fontData, fontDataSize);
	}

	initializeRendererTests();
}

application::Renderer::~Renderer() = default;

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
	for (auto& rendererTest : rendererTests) {
		rendererTest->render();
	}

	fpsCounter.frame();
}

void application::Renderer::initializeRendererTests() {
	rendererTests.emplace_back(std::make_unique<renderer_test::LayerRendererTest>(*this));
	rendererTests.emplace_back(std::make_unique<renderer_test::ShapeEffectRendererTest>(*this));
	rendererTests.emplace_back(std::make_unique<renderer_test::CanvasRendererTest>(*this));
	rendererTests.emplace_back(std::make_unique<renderer_test::TextureRendererTest>(*this));
	rendererTests.emplace_back(std::make_unique<renderer_test::TextEffectRendererTest>(*this));
	rendererTests.emplace_back(std::make_unique<renderer_test::FrameEffectRendererTest>(*this));
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
