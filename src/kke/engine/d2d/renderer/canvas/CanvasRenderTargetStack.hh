#pragma once

#include <memory>
#include <stack>

#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/canvas/D2dCanvas.hh"

namespace kke {
class CanvasRenderTargetStack {
	struct RenderTargetState {
		Microsoft::WRL::ComPtr<ID2D1Image> renderTarget;
		std::shared_ptr<D2dCanvas> canvas;
	};

	std::stack<RenderTargetState> renderTargetStack;

  public:
	void pushCanvas(D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas);

	void popCanvas(D2dEngineContext const& context);

  private:
	void
	pushCurrentRenderTarget(D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas);
};
}; // namespace kke
