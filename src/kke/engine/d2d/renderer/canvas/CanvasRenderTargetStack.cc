#include "CanvasRenderTargetStack.hh"

using namespace Microsoft::WRL;
using namespace kke;

void CanvasRenderTargetStack::pushCanvas(D2dContext const& context, std::shared_ptr<D2dCanvas> canvas) {
	pushCurrentRenderTarget(context);
	context.getDeviceContext()->SetTarget(canvas->getCommandList().Get());
}

void CanvasRenderTargetStack::popCanvas(D2dContext const& context) {
	if (renderTargetStack.empty()) {
		// TODO: Log error
		return;
	}
	ComPtr<ID2D1Image> target = renderTargetStack.top();
	context.getDeviceContext()->SetTarget(target.Get());
	renderTargetStack.pop();
}

void CanvasRenderTargetStack::pushCurrentRenderTarget(D2dContext const& context) {
	ComPtr<ID2D1Image> currentTarget;
	context.getDeviceContext()->GetTarget(&currentTarget);
	renderTargetStack.push(currentTarget);
}
