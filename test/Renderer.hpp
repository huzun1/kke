#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "backend/D2D1.hpp"
#include "oreik/Engine.hpp"

namespace application {
class Renderer {
	application::D2D1& d2d1;

	std::unique_ptr<oreik::Engine> engine;
	uint64_t dyconTexId;

public:
	Renderer(application::D2D1& d2d1);

	/**
	 * @brief Pre-setup
	 */
	void render();

private:
	/**
	 * @brief Pre-Render test with the default context
	 */
	void preRender();

	/**
	 * @brief Rendering a frame by using the engine instance
	 */
	void renderFrame();

	static std::pair<void*, size_t> loadResource(int resourceId);
};
};	// namespace application
