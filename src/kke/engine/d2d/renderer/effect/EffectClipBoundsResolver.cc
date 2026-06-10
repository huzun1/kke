#include "EffectClipBoundsResolver.hh"

#include <algorithm>

using namespace kke;

D2D1_RECT_F EffectClipBoundsResolver::resolve(EffectClipSource const& clip) {
	return std::visit([](auto const& clipVariant) -> D2D1_RECT_F { return resolve(clipVariant); }, clip);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(Geometry const& geometry) {
	return std::visit(
		[](auto const& geometryVariant) -> D2D1_RECT_F { return resolve(geometryVariant); },
		geometry
	);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(GeometryCompose const& clip) {
	float left = 0.0f;
	float top = 0.0f;
	float right = 0.0f;
	float bottom = 0.0f;
	bool first = true;

	for (Geometry const& geometry : clip.getGeometries()) {
		D2D1_RECT_F geometryBounds = resolve(geometry);
		if (first) {
			left = geometryBounds.left;
			top = geometryBounds.top;
			right = geometryBounds.right;
			bottom = geometryBounds.bottom;
			first = false;
			continue;
		}

		left = std::min(left, geometryBounds.left);
		top = std::min(top, geometryBounds.top);
		right = std::max(right, geometryBounds.right);
		bottom = std::max(bottom, geometryBounds.bottom);
	}

	return D2D1::RectF(left, top, right, bottom);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(Triangle const& triangle) {
	return D2D1::RectF(
		std::min({triangle.a.x, triangle.b.x, triangle.c.x}),
		std::min({triangle.a.y, triangle.b.y, triangle.c.y}),
		std::max({triangle.a.x, triangle.b.x, triangle.c.x}),
		std::max({triangle.a.y, triangle.b.y, triangle.c.y})
	);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(Rect const& rect) {
	return D2D1::RectF(rect.min.x, rect.min.y, rect.max.x, rect.max.y);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(RoundedRect const& roundedRect) {
	return D2D1::RectF(
		roundedRect.min.x,
		roundedRect.min.y,
		roundedRect.max.x,
		roundedRect.max.y
	);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(Ellipse const& ellipse) {
	return D2D1::RectF(
		ellipse.point.x - ellipse.radius,
		ellipse.point.y - ellipse.radius,
		ellipse.point.x + ellipse.radius,
		ellipse.point.y + ellipse.radius
	);
}

D2D1_RECT_F EffectClipBoundsResolver::resolve(Polygon const& polygon) {
	float left = 0.0f;
	float top = 0.0f;
	float right = 0.0f;
	float bottom = 0.0f;
	bool first = true;

	for (Point const& point : polygon.getPoints()) {
		if (first) {
			left = point.x;
			top = point.y;
			right = point.x;
			bottom = point.y;
			first = false;
			continue;
		}

		left = std::min(left, point.x);
		top = std::min(top, point.y);
		right = std::max(right, point.x);
		bottom = std::max(bottom, point.y);
	}

	return D2D1::RectF(left, top, right, bottom);
}
