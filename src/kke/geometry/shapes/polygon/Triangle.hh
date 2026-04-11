#pragma once

#include "kke/geometry/Boundary.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
struct Triangle : public Geometry {
    Point2f a;
    Point2f b;
    Point2f c;

    Triangle();

	Triangle(Point2f const& a, Point2f const& b, Point2f const& c);

    [[nodiscard]] Triangle shrink(float amount);

    [[nodiscard]] Triangle expand(float amount);

    Boundary getBounding() const override;
};

#include "TriangleOperators.hh"

};	// namespace kke
