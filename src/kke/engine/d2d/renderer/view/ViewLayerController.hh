#pragma once

#include <vector>

#include <wrl/client.h>

#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/D2dLayerStatistics.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Polygon.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

class ViewLayerController {
  private:
	enum class PushedLayerType {
		NoOperation,
		AxisAlignedClip,
		Layer,
	};

	std::vector<PushedLayerType> pushedLayerTypes;
	kke::D2dLayerStatistics statistics;

  public:
	void beginFrame();

	void pushLayer(
		kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
	);

	void popLayer(kke::D2dEngineContext const& context);

	kke::D2dLayerStatistics const& getStatistics() const;

  private:
	static kke::Rect const* axisAlignedClipRect(
		kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
	);

	Microsoft::WRL::ComPtr<ID2D1Geometry> createGeometry(
		kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
	);
};
