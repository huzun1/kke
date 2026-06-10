#include "CanvasRenderTargetStack.hh"

#include "kke/utils/DebugLog.hh"

using namespace Microsoft::WRL;
using namespace kke;

void CanvasRenderTargetStack::pushCanvas(
	D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas
) {
	D2dContext* d2dContext = context.getD2dContext();

	pushCurrentRenderTarget(context, canvas);
	d2dContext->getDeviceContext()->SetTarget(canvas->getCommandList().Get());
}

void CanvasRenderTargetStack::popCanvas(D2dEngineContext const& context) {
	if (renderTargetStack.empty()) {
		kke::debug::log("[kke][CanvasRenderTargetStack] popCanvas called with an empty stack");
		return;
	}

	D2dContext* d2dContext = context.getD2dContext();

	RenderTargetState state = renderTargetStack.top();
	d2dContext->getDeviceContext()->SetTarget(state.renderTarget.Get());
	state.canvas->close();
	renderTargetStack.pop();
}

void CanvasRenderTargetStack::pushCurrentRenderTarget(
	D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas
) {
	D2dContext* d2dContext = context.getD2dContext();

	ComPtr<ID2D1Image> currentTarget;
	d2dContext->getDeviceContext()->GetTarget(&currentTarget);
	renderTargetStack.push({currentTarget, canvas});
}
