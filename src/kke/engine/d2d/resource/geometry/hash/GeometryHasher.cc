#include <variant>

#include "GeometryHasher.hh"

#include "GeometryHashTag.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;

uint64_t GeometryHasher::hash(Geometry const& geometry, GeometryHashMode mode) {
	return std::visit([&](auto const& geo) {
		return hash(geo, mode);
	}, geometry);
}

uint64_t GeometryHasher::hash(GeometryCompose const& compose, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::Compose);

	for (auto const& geometry : compose.getGeometries()) {
		hasher.combine(hash(geometry, mode));
	}

	return hasher.get();
}

uint64_t GeometryHasher::hash(Triangle const& triangle, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::Triangle);

	if (mode == GeometryHashMode::PositionIndependent) {
		auto const base = triangle.a;
		auto const a = triangle.a - base;
		auto const b = triangle.b - base;
		auto const c = triangle.c - base;

		hasher.combine(a.x);
		hasher.combine(a.y);
		hasher.combine(b.x);
		hasher.combine(b.y);
		hasher.combine(c.x);
		hasher.combine(c.y);
	} else {
		hasher.combine(triangle.a.x);
		hasher.combine(triangle.a.y);
		hasher.combine(triangle.b.x);
		hasher.combine(triangle.b.y);
		hasher.combine(triangle.c.x);
		hasher.combine(triangle.c.y);
	}

	return hasher.get();
}

uint64_t GeometryHasher::hash(Rect const& rect, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::Rect);

	if (mode == GeometryHashMode::PositionIndependent) {
		auto const size = rect.max - rect.min;
		hasher.combine(size.x);
		hasher.combine(size.y);
	} else {
		hasher.combine(rect.min.x);
		hasher.combine(rect.min.y);
		hasher.combine(rect.max.x);
		hasher.combine(rect.max.y);
	}

	return hasher.get();
}

uint64_t GeometryHasher::hash(RoundedRect const& roundedRect, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::RoundedRect);

	if (mode == GeometryHashMode::PositionIndependent) {
		auto const size = roundedRect.max - roundedRect.min;

		hasher.combine(size.x);
		hasher.combine(size.y);
		hasher.combine(roundedRect.rounding);
	} else {
		hasher.combine(roundedRect.min.x);
		hasher.combine(roundedRect.min.y);
		hasher.combine(roundedRect.max.x);
		hasher.combine(roundedRect.max.y);
		hasher.combine(roundedRect.rounding);
	}

	return hasher.get();
}

uint64_t GeometryHasher::hash(Ellipse const& ellipse, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::Ellipse);

	if (mode == GeometryHashMode::PositionIndependent) {
		hasher.combine(ellipse.radius);
	} else {
		hasher.combine(ellipse.point.x);
		hasher.combine(ellipse.point.y);
		hasher.combine(ellipse.radius);
	}

	return hasher.get();
}

uint64_t GeometryHasher::hash(Polygon const& polygon, GeometryHashMode mode) {
	Hasher hasher;
	hasher.combine(GeometryHashTag::Polygon);

	auto const& points = polygon.getPoints();

	if (mode == GeometryHashMode::PositionIndependent) {
		if (points.empty()) {
			return hasher.get();
		}
		auto const base = points.front();
		for (auto const& point : points) {
			auto const relativePoint = point - base;
			hasher.combine(relativePoint.x);
			hasher.combine(relativePoint.y);
		}
	} else {
		for (auto const& point : points) {
			hasher.combine(point.x);
			hasher.combine(point.y);
		}
	}

	return hasher.get();
}
