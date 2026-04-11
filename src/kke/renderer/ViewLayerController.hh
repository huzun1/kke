#pragma once

#include <kke/geometry/Polygon.hh>
#include <kke/geometry/primitives/Point.hh>

class ViewLayerController {
public:
	void pushLayer(kke::Polygon const& mask);

	void popLayer();
};
