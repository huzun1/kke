#include "PositionIndependentEffectSource.hh"

#include <algorithm>

#include "kke/engine/d2d/resource/font/hash/FontHasher.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHashMode.hh"
#include "kke/engine/d2d/resource/geometry/hash/GeometryHasher.hh"
#include "kke/geometry/curved/EllipseOperators.hh"
#include "kke/geometry/primitives/PointOperators.hh"
#include "kke/geometry/shapes/RectOperators.hh"
#include "kke/geometry/shapes/TriangleOperators.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;

EffectSource
PositionIndependentEffectSource::normalize(EffectSource const& source, Point const& origin) {
	if (Line const* line = std::get_if<Line>(&source)) {
		return Line{line->start - origin, line->end - origin, line->thickness};
	}

	if (Geometry const* geometry = std::get_if<Geometry>(&source)) {
		return normalize(*geometry, origin);
	}

	if (GeometryCompose const* compose = std::get_if<GeometryCompose>(&source)) {
		GeometryCompose normalizedCompose;
		for (Geometry const& geometry : compose->getGeometries()) {
			normalizedCompose.add(normalize(geometry, origin));
		}
		return normalizedCompose;
	}

	Text const& text = std::get<Text>(source);
	return Text{text.text, text.position - origin, text.fontAppearance};
}

EffectClipSource
PositionIndependentEffectSource::normalizeClip(EffectClipSource const& clip, Point const& origin) {
	if (Geometry const* geometry = std::get_if<Geometry>(&clip)) {
		return normalize(*geometry, origin);
	}

	GeometryCompose normalizedCompose;
	for (Geometry const& geometry : std::get<GeometryCompose>(clip).getGeometries()) {
		normalizedCompose.add(normalize(geometry, origin));
	}
	return normalizedCompose;
}

Point PositionIndependentEffectSource::getOrigin(EffectSource const& source) {
	if (Line const* line = std::get_if<Line>(&source)) {
		return {std::min(line->start.x, line->end.x), std::min(line->start.y, line->end.y)};
	}

	if (Geometry const* geometry = std::get_if<Geometry>(&source)) {
		return getOrigin(*geometry);
	}

	if (GeometryCompose const* compose = std::get_if<GeometryCompose>(&source)) {
		float left = 0.0f;
		float top = 0.0f;
		bool first = true;
		for (Geometry const& geometry : compose->getGeometries()) {
			Point geometryOrigin = getOrigin(geometry);
			if (first) {
				left = geometryOrigin.x;
				top = geometryOrigin.y;
				first = false;
				continue;
			}

			left = std::min(left, geometryOrigin.x);
			top = std::min(top, geometryOrigin.y);
		}
		return {left, top};
	}

	if (Text const* text = std::get_if<Text>(&source)) {
		return text->position;
	}

	return {0.0f, 0.0f};
}

uint64_t PositionIndependentEffectSource::hash(EffectSource const& source) {
	if (Line const* line = std::get_if<Line>(&source)) {
		return hash(*line);
	}

	if (Geometry const* geometry = std::get_if<Geometry>(&source)) {
		return GeometryHasher::hash(*geometry, GeometryHashMode::PositionIndependent);
	}

	if (GeometryCompose const* compose = std::get_if<GeometryCompose>(&source)) {
		GeometryCompose normalizedCompose;
		Point origin = getOrigin(source);
		for (Geometry const& geometry : compose->getGeometries()) {
			normalizedCompose.add(normalize(geometry, origin));
		}
		return GeometryHasher::hash(normalizedCompose, GeometryHashMode::PositionDependent);
	}

	return FontHasher::hash(std::get<Text>(source));
}

Geometry PositionIndependentEffectSource::normalize(Geometry const& geometry, Point const& origin) {
	return std::visit(
		[&](auto const& geometryVariant) -> Geometry { return normalize(geometryVariant, origin); },
		geometry
	);
}

Triangle PositionIndependentEffectSource::normalize(Triangle const& triangle, Point const& origin) {
	return triangle - origin;
}

Rect PositionIndependentEffectSource::normalize(Rect const& rect, Point const& origin) {
	return rect - origin;
}

RoundedRect
PositionIndependentEffectSource::normalize(RoundedRect const& roundedRect, Point const& origin) {
	return {Rect{roundedRect} - origin, roundedRect.rounding};
}

Ellipse PositionIndependentEffectSource::normalize(Ellipse const& ellipse, Point const& origin) {
	return ellipse - origin;
}

Polygon PositionIndependentEffectSource::normalize(Polygon const& polygon, Point const& origin) {
	std::vector<Point> normalizedPoints;
	for (Point const& point : polygon.getPoints()) {
		normalizedPoints.push_back(point - origin);
	}
	return {normalizedPoints};
}

Point PositionIndependentEffectSource::getOrigin(Geometry const& geometry) {
	return std::visit(
		[](auto const& geometryVariant) -> Point { return getOrigin(geometryVariant); },
		geometry
	);
}

Point PositionIndependentEffectSource::getOrigin(Triangle const& triangle) {
	return {
		std::min({triangle.a.x, triangle.b.x, triangle.c.x}),
		std::min({triangle.a.y, triangle.b.y, triangle.c.y})
	};
}

Point PositionIndependentEffectSource::getOrigin(Rect const& rect) {
	return rect.min;
}

Point PositionIndependentEffectSource::getOrigin(RoundedRect const& roundedRect) {
	return roundedRect.min;
}

Point PositionIndependentEffectSource::getOrigin(Ellipse const& ellipse) {
	return {ellipse.point.x - ellipse.radius, ellipse.point.y - ellipse.radius};
}

Point PositionIndependentEffectSource::getOrigin(Polygon const& polygon) {
	float left = 0.0f;
	float top = 0.0f;
	bool first = true;
	for (Point const& point : polygon.getPoints()) {
		if (first) {
			left = point.x;
			top = point.y;
			first = false;
			continue;
		}

		left = std::min(left, point.x);
		top = std::min(top, point.y);
	}
	return {left, top};
}

uint64_t PositionIndependentEffectSource::hash(Line const& line) {
	Hasher hasher;
	Point base{std::min(line.start.x, line.end.x), std::min(line.start.y, line.end.y)};
	hasher.combine((line.start - base).x);
	hasher.combine((line.start - base).y);
	hasher.combine((line.end - base).x);
	hasher.combine((line.end - base).y);
	hasher.combine(line.thickness);
	return hasher.get();
}
