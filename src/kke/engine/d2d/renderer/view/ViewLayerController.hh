#pragma once

#include "kke/geometry/Maskable.hh"
#include "kke/appearance/view/LayerMode.hh"

class ViewLayerController {
public:
	void pushLayer(kke::Maskable const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void popLayer();
};
