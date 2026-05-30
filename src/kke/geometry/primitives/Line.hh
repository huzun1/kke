#pragma once

#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/Boundable.hh"

namespace kke {
class Line : public Boundable {
public:
    kke::Point start, end;
    float thickness;

    Line();

    Line(kke::Point const& start, kke::Point const& end, float thickness);

    [[nodiscard]] Boundary getBounding() const override;
};
};	// namespace kke
