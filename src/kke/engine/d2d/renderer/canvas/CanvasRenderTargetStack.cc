#include "CanvasRenderTargetStack.hh"

#include "kke/utils/DebugLog.hh"

using namespace Microsoft::WRL;
using namespace kke;

bool CanvasRenderTargetStack::pushCanvas(
	D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas
) {
	D2dContext* d2dContext = context.getD2dContext();

	pushCurrentRenderTarget(context, canvas);
	d2dContext->getDeviceContext()->SetTarget(canvas->getCommandList().Get());
	d2dContext->getDeviceContext()->SetTransform(D2D1::Matrix3x2F::Identity());
	return true;
}

bool CanvasRenderTargetStack::popCanvas(D2dEngineContext const& context) {
	if (renderTargetStack.empty()) {
		kke::debug::log("[kke][CanvasRenderTargetStack] popCanvas called with an empty stack");
		return false;
	}

	D2dContext* d2dContext = context.getD2dContext();

	RenderTargetState state = renderTargetStack.top();
	d2dContext->getDeviceContext()->SetTarget(state.renderTarget.Get());
	d2dContext->getDeviceContext()->SetTransform(state.transform);
	state.canvas->close();
	renderTargetStack.pop();
	return true;
}

bool CanvasRenderTargetStack::suspendCanvas(D2dEngineContext const& context) {
	if (renderTargetStack.empty()) {
		return false;
	}

	RenderTargetState const& state = renderTargetStack.top();
	D2dContext* d2dContext = context.getD2dContext();
	d2dContext->getDeviceContext()->SetTarget(state.renderTarget.Get());
	d2dContext->getDeviceContext()->SetTransform(state.transform);
	return true;
}

bool CanvasRenderTargetStack::resumeCanvas(D2dEngineContext const& context) {
	if (renderTargetStack.empty()) {
		return false;
	}

	D2dContext* d2dContext = context.getD2dContext();
	d2dContext->getDeviceContext()->SetTarget(renderTargetStack.top().canvas->getCommandList().Get()
	);
	d2dContext->getDeviceContext()->SetTransform(D2D1::Matrix3x2F::Identity());
	return true;
}

void CanvasRenderTargetStack::pushCurrentRenderTarget(
	D2dEngineContext const& context, std::shared_ptr<D2dCanvas> canvas
) {
	D2dContext* d2dContext = context.getD2dContext();

	ComPtr<ID2D1Image> currentTarget;
	D2D1_MATRIX_3X2_F currentTransform;
	d2dContext->getDeviceContext()->GetTarget(&currentTarget);
	d2dContext->getDeviceContext()->GetTransform(&currentTransform);
	renderTargetStack.push({currentTarget, currentTransform, canvas});
}
