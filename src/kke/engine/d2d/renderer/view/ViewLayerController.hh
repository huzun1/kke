#pragma once

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Polygon.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

class ViewLayerController {
public:
	void pushLayer(kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode);

	void popLayer(kke::D2dEngineContext const& context);

private:
	void pushLayer(kke::D2dEngineContext const& context, kke::Triangle const& mask, kke::LayerMode mode);

	void pushLayer(kke::D2dEngineContext const& context, kke::Rect const& mask, kke::LayerMode mode);

	void pushLayer(kke::D2dEngineContext const& context, kke::RoundedRect const& mask, kke::LayerMode mode);

	void pushLayer(kke::D2dEngineContext const& context, kke::Ellipse const& mask, kke::LayerMode mode);

	void pushLayer(kke::D2dEngineContext const& context, kke::Polygon const& mask, kke::LayerMode mode);

	void pushLayer(kke::D2dEngineContext const& context, kke::GeometryCompose const& mask, kke::LayerMode mode);
};
