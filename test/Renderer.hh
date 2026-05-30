#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
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
	application::D2D1& d2d1;
	kke::D2dEngine engine;
	FpsCounter fpsCounter;
	std::shared_ptr<kke::Texture> encodedTexture;
	std::shared_ptr<kke::Texture> rawTexture;
	std::vector<uint8_t> rawTexturePixels;
	std::vector<std::unique_ptr<renderer_test::RendererTest>> rendererTests;

public:
	Renderer(application::D2D1& d2d1);

	~Renderer();

	void render();

private:
	void ensureTexturesUploaded();

	void renderFrame();

	void initializeRendererTests();

	static std::vector<uint8_t> createRawTexturePixels(uint32_t width, uint32_t height);

	static std::pair<void const*, size_t> loadResource(int resourceId);

	friend class renderer_test::RendererTest;
};
}	// namespace application
