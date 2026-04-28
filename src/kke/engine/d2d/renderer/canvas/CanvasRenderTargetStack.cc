#include "CanvasRenderTargetStack.hh"

using namespace Microsoft::WRL;
using namespace kke;

void CanvasRenderTargetStack::pushCanvas(D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas) {
	D2dContext* d2dContext = context.getD2dContext();

	pushCurrentRenderTarget(context);
	d2dContext->getDeviceContext()->SetTarget(canvas->getCommandList().Get());
}

void CanvasRenderTargetStack::popCanvas(D2dEngineContext const& context) {
	if (renderTargetStack.empty()) {
		// TODO: Log error
		return;
	}

	D2dContext* d2dContext = context.getD2dContext();

	ComPtr<ID2D1Image> target = renderTargetStack.top();
	d2dContext->getDeviceContext()->SetTarget(target.Get());
	renderTargetStack.pop();
}

void CanvasRenderTargetStack::pushCurrentRenderTarget(D2dEngineContext const& context) {
	D2dContext* d2dContext = context.getD2dContext();

	ComPtr<ID2D1Image> currentTarget;
	d2dContext->getDeviceContext()->GetTarget(&currentTarget);
	renderTargetStack.push(currentTarget);
}
