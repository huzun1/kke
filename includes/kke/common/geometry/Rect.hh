#pragma once

#include <d2d1.h>

#include <cstdint>

#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"

namespace kke {
struct Rect : public Geometry {
	float x1, y1, x2, y2;

	Rect(float x1, float y1, float x2, float y2);

	void create(ID2D1Factory* factory, ID2D1Geometry** output) const override;

	uint64_t hash(bool positionDependent) const override;

	float width() const;

	float height() const;

	kke::Point2f center() const;

	void offset(float x1, float y1, float x2, float y2);

	D2D1_RECT_F rectF() const;
};
};	// namespace kke
