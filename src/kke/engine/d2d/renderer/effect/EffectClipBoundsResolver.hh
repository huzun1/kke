#pragma once

#include "kke/engine/Sources.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class EffectClipBoundsResolver {
  public:
	static D2D1_RECT_F resolve(EffectClipSource const& clip);

  private:
	static D2D1_RECT_F resolve(Geometry const& geometry);

	static D2D1_RECT_F resolve(GeometryCompose const& clip);

	static D2D1_RECT_F resolve(Triangle const& triangle);

	static D2D1_RECT_F resolve(Rect const& rect);

	static D2D1_RECT_F resolve(RoundedRect const& roundedRect);

	static D2D1_RECT_F resolve(Ellipse const& ellipse);

	static D2D1_RECT_F resolve(Polygon const& polygon);
};
} // namespace kke
