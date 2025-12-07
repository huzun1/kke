#pragma once

#include <d2d1.h>

#include <cstdint>
#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"

namespace kke {
struct Triangle : public Geometry {
    Point2f a;
    Point2f b;
    Point2f c;

    Triangle();

	Triangle(Point2f const& a, Point2f const& b, Point2f const& c);

	void create(ID2D1Factory* factory, ID2D1Geometry** output) const override;

	uint64_t hash(bool positionDependent) const override;
};
};	// namespace kke
