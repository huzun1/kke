#pragma once

#include "kke/geometry/Boundary.hh"
#include "kke/geometry/Polygon.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Triangle : public Polygon {
    Point2f a;
    Point2f b;
    Point2f c;

    Triangle();

	Triangle(Point2f const& a, Point2f const& b, Point2f const& c);

    [[nodiscard]] Boundary getBounding() const override;

    [[nodiscard]] Triangle shrink(float amount) const;

    [[nodiscard]] Triangle expand(float amount) const;
};

#include "TriangleOperators.hh"

};	// namespace kke
