#pragma once

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class FaceRenderer {
  public:
	void fill(D2dEngineContext const& context, FillSource const& source, Brush const& brush);

  private:
	void fill(D2dEngineContext const& context, Geometry const& geometry, ID2D1Brush* brush);

	void fill(D2dEngineContext const& context, GeometryCompose const& compose, ID2D1Brush* brush);

	void fill(D2dEngineContext const& context, Text const& text, ID2D1Brush* brush);
};
} // namespace kke
