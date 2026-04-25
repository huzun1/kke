#pragma once

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Polygon.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

class ViewLayerController {
public:
	void pushLayer(kke::Triangle const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void pushLayer(kke::Rect const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void pushLayer(kke::RoundedRect const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void pushLayer(kke::Ellipse const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void pushLayer(kke::Polygon const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void pushLayer(kke::GeometryCompose const& mask, kke::LayerMode mode = kke::LayerMode::NORMAL);

	void popLayer();
};
