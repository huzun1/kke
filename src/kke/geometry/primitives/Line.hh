#pragma once

#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/Geometry.hh"

namespace kke {
class Line : public Geometry {
public:
    kke::Point2f start, end;

    Line();

    Line(kke::Point2f const& start, kke::Point2f const& end);

    Boundary getBounding() const override;
};
};	// namespace kke
