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

        hasher.combine(triangle.a - base);
        hasher.combine(triangle.b - base);
        hasher.combine(triangle.c - base);
    } else {
        hasher.combine(triangle.a);
        hasher.combine(triangle.b);
        hasher.combine(triangle.c);
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
            hasher.combine(point - base);
        }
    } else {
        for (auto const& point : points) {
            hasher.combine(point);
        }
    }

    return hasher.get();
}