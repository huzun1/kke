#pragma once

#include "kke/common/Point.hh"
#include "kke/transform/Transform.hh"

namespace kke {
class RotateTransform : public Transform {
	kke::Point center;
	float angle;

public:
	RotateTransform(kke::Point const& center, float angle);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
