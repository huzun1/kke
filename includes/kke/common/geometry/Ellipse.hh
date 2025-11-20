#pragma once

#include <d2d1.h>

#include <cstdint>

#include "Rect.hh"
#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"

namespace kke {
struct Ellipse : public Geometry {
	kke::Point2f point;
	float radius;

	Ellipse(float x, float y, float radius);

	void create(ID2D1Factory* factory, ID2D1Geometry** output) const override;

	uint64_t hash(bool positionDependent) const override;

	D2D1_ELLIPSE ellipse() const;

	kke::Rect dimension() const;
};
};	// namespace kke
