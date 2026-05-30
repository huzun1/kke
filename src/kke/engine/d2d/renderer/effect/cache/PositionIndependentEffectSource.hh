#pragma once

#include "kke/engine/Sources.hh"

namespace kke {
class PositionIndependentEffectSource {
public:
	static EffectSource normalize(
		EffectSource const& source,
		Point const& origin);

	static EffectClipSource normalizeClip(
		EffectClipSource const& clip,
		Point const& origin);

	static Point getOrigin(EffectSource const& source);

	static uint64_t hash(EffectSource const& source);

private:
	static Geometry normalize(
		Geometry const& geometry,
		Point const& origin);

	static Triangle normalize(
		Triangle const& triangle,
		Point const& origin);

	static Rect normalize(
		Rect const& rect,
		Point const& origin);

	static RoundedRect normalize(
		RoundedRect const& roundedRect,
		Point const& origin);

	static Ellipse normalize(
		Ellipse const& ellipse,
		Point const& origin);

	static Polygon normalize(
		Polygon const& polygon,
		Point const& origin);

	static Point getOrigin(Geometry const& geometry);

	static Point getOrigin(Triangle const& triangle);

	static Point getOrigin(Rect const& rect);

	static Point getOrigin(RoundedRect const& roundedRect);

	static Point getOrigin(Ellipse const& ellipse);

	static Point getOrigin(Polygon const& polygon);

	static uint64_t hash(Line const& line);
};
}	// namespace kke
