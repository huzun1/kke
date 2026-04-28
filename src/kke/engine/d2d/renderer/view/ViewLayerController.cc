#include "ViewLayerController.hh"

#include <variant>

void ViewLayerController::pushLayer(kke::D2dContext const& context, kke::MaskSource const& mask, kke::LayerMode mode) {
	std::visit([&](auto&& mask) {
		pushLayer(context, mask, mode);
	}, mask);
}

void ViewLayerController::popLayer(kke::D2dContext const& context) {
    context.getDeviceContext()->PopLayer();
}

