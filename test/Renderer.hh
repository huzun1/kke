#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "backend/D2D1.hh"
#include "kke/Engine.hh"
#include "kke/ui/Component.hh"

namespace application {
class Renderer {
   	std::unique_ptr<kke::Engine> engine;
	application::D2D1& d2d1;
	uint64_t dyconTexId;

	// UI Builder components
	std::shared_ptr<kke::ui::Component> uiComponent;

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

	/**
	 * @brief Initialize UI components using the builder pattern
	 */
	void initializeUI();

	static std::pair<void*, size_t> loadResource(int resourceId);
};
};	// namespace application
