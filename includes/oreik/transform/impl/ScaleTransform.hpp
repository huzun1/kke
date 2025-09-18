#pragma once

#include "oreik/common/Point.hpp"
#include "oreik/common/Scale.hpp"
#include "oreik/transform/Transform.hpp"

namespace oreik {
class ScaleTransform : public Transform {
	oreik::Point2f center;
	oreik::Scale2f scale;

public:
	ScaleTransform(oreik::Point2f center, oreik::Scale2f scale);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace oreik
