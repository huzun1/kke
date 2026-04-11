#pragma once

#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/Boundable.hh"

namespace kke {
class Line : public Boundable {
public:
    kke::Point2f start, end;

    Line();

    Line(kke::Point2f const& start, kke::Point2f const& end);

    [[nodiscard]] Boundary getBounding() const override;
};
};	// namespace kke
