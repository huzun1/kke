#include "CanvasService.hh"
#include <memory>

#include "kke/engine/d2d/resource/canvas/CanvasFactory.hh"
#include "kke/utils/DebugLog.hh"

using namespace kke;

std::shared_ptr<D2dCanvas> CanvasService::createCanvas(D2dEngineContext const& context) {
	return CanvasFactory::createCanvas(context);
}

bool CanvasService::beginCanvas(D2dEngineContext const& context, std::shared_ptr<Canvas> canvas) {
	std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(canvas);
	if (!d2dCanvas) {
		kke::debug::log("[kke][CanvasService] beginCanvas received an invalid canvas type");
		return false;
	}
	if (d2dCanvas->closed()) {
		kke::debug::log("[kke][CanvasService] beginCanvas received a finalized canvas");
		return false;
	}
	return renderTargetStack.beginCanvas(context, d2dCanvas);
}

bool CanvasService::endCanvas(D2dEngineContext const& context) {
	return renderTargetStack.endCanvas(context);
}

bool CanvasService::finishCanvas(std::shared_ptr<Canvas> canvas) {
	std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(canvas);
	if (!d2dCanvas) {
		kke::debug::log("[kke][CanvasService] finishCanvas received an invalid canvas type");
		return false;
	}
	if (!d2dCanvas->close()) {
		kke::debug::log("[kke][CanvasService] failed to finalize canvas");
		return false;
	}
	return true;
}

void CanvasService::drawCanvas(
	D2dEngineContext const& context, std::shared_ptr<Canvas> canvas, float opacity
) {
	std::shared_ptr<D2dCanvas> d2dCanvas = std::dynamic_pointer_cast<D2dCanvas>(canvas);
	if (!d2dCanvas) {
		kke::debug::log("[kke][CanvasService] drawCanvas received an invalid canvas type");
		return;
	}

	if (!d2dCanvas->closed()) {
		kke::debug::log("[kke][CanvasService] drawCanvas received an unfinished canvas");
		return;
	}

	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	if (opacity >= 1.0f) {
		deviceContext->DrawImage(d2dCanvas->getCommandList().Get());
		return;
	}

	D2D1_LAYER_PARAMETERS1 layerParameters = D2D1::LayerParameters1(
		D2D1::InfiniteRect(),
		nullptr,
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
		D2D1::IdentityMatrix(),
		opacity
	);
	deviceContext->PushLayer(layerParameters, nullptr);
	deviceContext->DrawImage(d2dCanvas->getCommandList().Get());
	deviceContext->PopLayer();
}
