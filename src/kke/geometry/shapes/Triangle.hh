#pragma once

#include "kke/geometry/Boundable.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
struct Triangle : public Boundable {
    Point a;
    Point b;
    Point c;

    ~Triangle() = default;

    Triangle();

	Triangle(Point const& a, Point const& b, Point const& c);

    [[nodiscard]] Boundary getBounding() const override;

    [[nodiscard]] Triangle shrink(float amount) const;

    [[nodiscard]] Triangle expand(float amount) const;
};

#include "TriangleOperators.hh"

};	// namespace kke
