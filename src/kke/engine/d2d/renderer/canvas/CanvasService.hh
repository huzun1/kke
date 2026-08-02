#pragma once

#include <memory>

#include "CanvasRenderTargetStack.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class CanvasService {
	CanvasRenderTargetStack renderTargetStack;

  public:
	std::shared_ptr<D2dCanvas> createCanvas(D2dEngineContext const& context);

	bool beginCanvas(D2dEngineContext const& context, std::shared_ptr<Canvas> canvas);

	bool endCanvas(D2dEngineContext const& context);

	bool finishCanvas(std::shared_ptr<Canvas> canvas);

	void drawCanvas(D2dEngineContext const& context, std::shared_ptr<Canvas> canvas, float opacity);
};
}; // namespace kke
