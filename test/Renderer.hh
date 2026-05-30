#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "FpsCounter.hh"
#include "backend/D2D1.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/d2d/D2dEngine.hh"

namespace application {
namespace renderer_test {
class RendererTest;
}
class Renderer {
	struct RendererTestEntry {
		uint32_t virtualKey = 0;
		std::string title;
		std::unique_ptr<renderer_test::RendererTest> rendererTest;
	};

	application::D2D1& d2d1;
	kke::D2dEngine engine;
	FpsCounter fpsCounter;
	std::shared_ptr<kke::Texture> encodedTexture;
	std::shared_ptr<kke::Texture> rawTexture;
	std::vector<uint8_t> rawTexturePixels;
	std::vector<RendererTestEntry> rendererTests;
	size_t activeRendererTestIndex = 0;
	bool hasActiveRendererTest = false;
	std::chrono::steady_clock::time_point lastFpsLogTime = std::chrono::steady_clock::now();

public:
	Renderer(application::D2D1& d2d1);

	~Renderer();

	void render();

	void handleKeyDown(uint32_t virtualKey);

private:
	void ensureTexturesUploaded();

	void renderFrame();

	void renderActiveRendererTest();

	void renderTestSelectorOverlay();

	void renderFpsOverlay();

	void logFps();

	void initializeRendererTests();

	void addRendererTest(
		uint32_t virtualKey,
		std::string title,
		std::unique_ptr<renderer_test::RendererTest> rendererTest);

	void switchRendererTest(size_t index);

	static std::vector<uint8_t> createRawTexturePixels(uint32_t width, uint32_t height);

	static std::pair<void const*, size_t> loadResource(int resourceId);

	friend class renderer_test::RendererTest;
};
}	// namespace application
