#include "Renderer.hh"

#include <cstdio>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h>

#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/appearance/Text.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "renderer_test/BlurStressRendererTest.hh"
#include "renderer_test/CanvasRendererTest.hh"
#include "renderer_test/FrameEffectRendererTest.hh"
#include "renderer_test/LayerRendererTest.hh"
#include "renderer_test/ShadowStressRendererTest.hh"
#include "renderer_test/ShapeEffectRendererTest.hh"
#include "renderer_test/TextEffectRendererTest.hh"
#include "renderer_test/TransformRendererTest.hh"
#include "renderer_test/TextureRendererTest.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"
#include "kke/geometry/shapes/Rect.hh"
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

void application::Renderer::handleKeyDown(uint32_t virtualKey) {
	if (virtualKey >= VK_NUMPAD1 && virtualKey <= VK_NUMPAD9) {
		virtualKey = '1' + (virtualKey - VK_NUMPAD1);
	}

	for (size_t index = 0; index < rendererTests.size(); ++index) {
		if (rendererTests[index].virtualKey != virtualKey) {
			continue;
		}

		switchRendererTest(index);
		return;
	}
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
	renderActiveRendererTest();
	renderTestSelectorOverlay();
	fpsCounter.frame();
	renderFpsOverlay();
	logFps();
}

void application::Renderer::renderActiveRendererTest() {
	if (rendererTests.empty()) {
		return;
	}

	rendererTests[activeRendererTestIndex].rendererTest->render();
}

void application::Renderer::renderTestSelectorOverlay() {
	if (rendererTests.empty()) {
		return;
	}

	kke::Brush panelFill = kke::SolidColorBrush({0.05f, 0.07f, 0.10f, 0.82f});
	kke::Brush titleBrush = kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 1.0f});
	kke::Brush activeBrush = kke::SolidColorBrush({0.56f, 0.84f, 1.0f, 1.0f});
	kke::Brush inactiveBrush = kke::SolidColorBrush({0.80f, 0.84f, 0.88f, 1.0f});

	constexpr float panelLeft = 24.0f;
	constexpr float panelTop = 24.0f;
	constexpr float panelWidth = 360.0f;
	constexpr float titleHeight = 28.0f;
	constexpr float lineHeight = 22.0f;
	constexpr float panelPadding = 18.0f;

	float panelHeight = panelPadding * 2.0f + titleHeight + rendererTests.size() * lineHeight;

	engine.fill(
		kke::Geometry{kke::Rect{
			{panelLeft, panelTop},
			{panelLeft + panelWidth, panelTop + panelHeight}
		}},
		panelFill);

	engine.fill(
		kke::Text{
			L"Renderer Tests",
			{panelLeft + panelPadding, panelTop + panelPadding},
			{"Space Grotesk", 24.0f, kke::FontWeight::BOLD}
		},
		titleBrush);

	for (size_t index = 0; index < rendererTests.size(); ++index) {
		RendererTestEntry const& entry = rendererTests[index];
		std::wstring line = L"[";
		line += static_cast<wchar_t>(entry.virtualKey);
		line += L"] ";
		line.append(entry.title.begin(), entry.title.end());

		engine.fill(
			kke::Text{
				line,
				{
					panelLeft + panelPadding,
					panelTop + panelPadding + titleHeight + index * lineHeight
				},
				{"Space Grotesk", 16.0f, index == activeRendererTestIndex ? kke::FontWeight::BOLD : kke::FontWeight::MEDIUM}
			},
			index == activeRendererTestIndex ? activeBrush : inactiveBrush);
	}
}

void application::Renderer::renderFpsOverlay() {
	std::ostringstream stream;
	stream.setf(std::ios::fixed);
	stream.precision(1);
	stream << "FPS " << fpsCounter.fps();

	std::string fpsString = stream.str();
	std::wstring fpsTextString(fpsString.begin(), fpsString.end());

	kke::Text fpsText{
		fpsTextString,
		{0.0f, 0.0f},
		{"Space Grotesk", 18.0f, kke::FontWeight::BOLD}
	};

	kke::Scale viewportSize = engine.getViewportSize();
	kke::Scale textSize = engine.measureTextSize(fpsText);

	constexpr float panelMargin = 24.0f;
	constexpr float panelInsetX = 14.0f;
	constexpr float panelInsetY = 10.0f;

	float panelLeft = viewportSize.x - textSize.x - panelInsetX * 2.0f - panelMargin;
	float panelTop = panelMargin;
	float panelRight = viewportSize.x - panelMargin;
	float panelBottom = panelTop + textSize.y + panelInsetY * 2.0f;

	kke::Brush panelFill = kke::SolidColorBrush({0.04f, 0.05f, 0.08f, 0.82f});
	kke::Brush textBrush = kke::SolidColorBrush({0.98f, 0.99f, 1.0f, 1.0f});

	engine.fill(
		kke::Geometry{kke::Rect{
			{panelLeft, panelTop},
			{panelRight, panelBottom}
		}},
		panelFill);

	fpsText.position = {
		panelLeft + panelInsetX,
		panelTop + panelInsetY - 2.0f
	};

	engine.fill(fpsText, textBrush);
}

void application::Renderer::logFps() {
	using clock = std::chrono::steady_clock;
	using seconds = std::chrono::duration<float>;

	const auto now = clock::now();
	if (seconds(now - lastFpsLogTime).count() < 1.0f) {
		return;
	}

	lastFpsLogTime = now;
	kke::PositionIndependentEffectCache::StatsSnapshot cacheStats = kke::PositionIndependentEffectCache::consumeStats();
	std::printf(
		"fps: %.2f, cache hits: %llu, cache misses: %llu\n",
		fpsCounter.fps(),
		static_cast<unsigned long long>(cacheStats.hits),
		static_cast<unsigned long long>(cacheStats.misses));
}

void application::Renderer::initializeRendererTests() {
	addRendererTest('1', "Blur Stress", std::make_unique<renderer_test::BlurStressRendererTest>(*this));
	addRendererTest('2', "Shadow Stress", std::make_unique<renderer_test::ShadowStressRendererTest>(*this));
	addRendererTest('3', "Shape Effect", std::make_unique<renderer_test::ShapeEffectRendererTest>(*this));
	addRendererTest('4', "Text Effect", std::make_unique<renderer_test::TextEffectRendererTest>(*this));
	addRendererTest('5', "Frame Effect", std::make_unique<renderer_test::FrameEffectRendererTest>(*this));
	addRendererTest('6', "Canvas", std::make_unique<renderer_test::CanvasRendererTest>(*this));
	addRendererTest('7', "Layer", std::make_unique<renderer_test::LayerRendererTest>(*this));
	addRendererTest('8', "Texture", std::make_unique<renderer_test::TextureRendererTest>(*this));
	addRendererTest('9', "Transform", std::make_unique<renderer_test::TransformRendererTest>(*this));

	switchRendererTest(0);
}

void application::Renderer::addRendererTest(
	uint32_t virtualKey,
	std::string title,
	std::unique_ptr<renderer_test::RendererTest> rendererTest) {
	rendererTests.emplace_back(RendererTestEntry{
		virtualKey,
		std::move(title),
		std::move(rendererTest)
	});
}

void application::Renderer::switchRendererTest(size_t index) {
	if (index >= rendererTests.size()) {
		return;
	}

	if (hasActiveRendererTest && activeRendererTestIndex == index) {
		return;
	}

	activeRendererTestIndex = index;
	hasActiveRendererTest = true;

	RendererTestEntry const& activeTest = rendererTests[activeRendererTestIndex];
	std::printf(
		"renderer test switched: [%c] %s\n",
		static_cast<int>(activeTest.virtualKey),
		activeTest.title.c_str());
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
