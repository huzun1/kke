#pragma once

#include "kke/geometry/Polygon.hh"
#include "kke/appearance/view/LayerMode.hh"

class ViewLayerController {
public:
	void pushLayer(kke::Polygon const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void popLayer();
};
