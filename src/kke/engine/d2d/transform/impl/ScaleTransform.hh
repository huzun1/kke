#pragma once

#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"
#include "kke/transform/Transform.hh"

namespace kke {
class ScaleTransform : public Transform {
	kke::Point center;
	kke::Scale scale;

public:
	ScaleTransform(kke::Point center, kke::Scale scale);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
