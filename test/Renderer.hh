#pragma once

#include <cstddef>
#include <utility>

#include "FpsCounter.hh"
#include "backend/D2D1.hh"
#include "kke/engine/d2d/D2dEngine.hh"

namespace application {
class Renderer {
	application::D2D1& d2d1;
	kke::D2dEngine engine;
	FpsCounter fpsCounter;

public:
	Renderer(application::D2D1& d2d1);

	void render();

private:
	void renderFrame();

	static std::pair<void const*, size_t> loadResource(int resourceId);
};
}	// namespace application
