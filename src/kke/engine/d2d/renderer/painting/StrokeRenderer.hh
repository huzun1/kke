#pragma once

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/primitives/Line.hh"

namespace kke {
class StrokeRenderer {
  public:
	void draw(
		D2dEngineContext const& context,
		StrokeSource const& source,
		Brush const& brush,
		StrokeAppearance const& appearance
	);

  private:
	void draw(
		D2dEngineContext const& context,
		Line const& line,
		ID2D1Brush* brush,
		StrokeAppearance const& appearance
	);

	void draw(
		D2dEngineContext const& context,
		Geometry const& geometry,
		ID2D1Brush* brush,
		StrokeAppearance const& appearance
	);

	void draw(
		D2dEngineContext const& context,
		GeometryCompose const& compose,
		ID2D1Brush* brush,
		StrokeAppearance const& appearance
	);
};
} // namespace kke
