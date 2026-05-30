#pragma once

#include <vector>

#include "kke/geometry/Boundable.hh"
#include "kke/geometry/Boundary.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
class Polygon : public Boundable {
	std::vector<Point> points;

public:
    ~Polygon() = default;

	Polygon(std::vector<Point> points);

    void addPoint(Point point);

    std::vector<Point> const& getPoints() const;

    Boundary getBounding() const override; 
};
};	// namespace kke
