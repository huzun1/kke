#pragma once

#include "kke/common/Point.hpp"
#include "kke/transform/Transform.hpp"
namespace kke {
class RotateTransform : public Transform {
	kke::Point2f center;
	float angle;

public:
	RotateTransform(kke::Point2f const& center, float angle);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
