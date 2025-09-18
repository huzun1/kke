#pragma once

#include "oreik/common/Point.hpp"
#include "oreik/transform/Transform.hpp"
namespace oreik {
class RotateTransform : public Transform {
	oreik::Point2f center;
	float angle;

public:
	RotateTransform(oreik::Point2f const& center, float angle);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace oreik
