#include "ViewLayerController.hh"

#include <variant>

#include "kke/appearance/view/LayerMode.hh"

using Microsoft::WRL::ComPtr;

void ViewLayerController::pushLayer(kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode) {
	ComPtr<ID2D1Geometry> geometry = createGeometry(context, mask, mode);
	if (!geometry) {
		// TODO: Log error
		return;
	}

	context.getD2dContext()->getDeviceContext()->PushLayer(
		D2D1::LayerParameters1(D2D1::InfiniteRect(), geometry.Get()),
		nullptr);
}

void ViewLayerController::popLayer(kke::D2dEngineContext const& context) {
	context.getD2dContext()->getDeviceContext()->PopLayer();
}

ComPtr<ID2D1Geometry> ViewLayerController::createGeometry(kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode) {
	// NOTE: If we will use kke::LayerMode in another place, we should close the switch statement to another file
	switch (mode) {
		case kke::LayerMode::Normal:
			return std::visit([&](auto const& maskVariant) {
				return context.getResourceProviders()->getGeometryProvider()->get(*context.getD2dContext(), maskVariant);
			}, mask);
		case kke::LayerMode::Inverted:
			return std::visit([&](auto const& maskVariant) {
				return context.getResourceProviders()->getInvertedGeometryProvider()->get(*context.getD2dContext(),
																						  *context.getResourceProviders()->getGeometryProvider(),
																						  maskVariant);
			}, mask);
		default:
			// TODO: Log error
			return nullptr;
	}
}
