#pragma once

#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"
#include "kke/transform/Transform.hh"

namespace kke {
class ScaleTransform : public Transform {
	kke::Point2f center;
	kke::Scale2f scale;

public:
	ScaleTransform(kke::Point2f center, kke::Scale2f scale);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
