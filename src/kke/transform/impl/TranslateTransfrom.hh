#pragma once

#include "kke/common/Point.hh"
#include "kke/transform/Transform.hh"

namespace kke {
class TranslateTransform : public Transform {
	kke::Point2f offset;

public:
	TranslateTransform(kke::Point2f offset);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
