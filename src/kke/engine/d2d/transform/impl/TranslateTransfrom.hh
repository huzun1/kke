#pragma once

#include "kke/geometry/primitives/Point.hh"

namespace kke {
class TranslateTransform : public Transform {
	kke::Point offset;

public:
	TranslateTransform(kke::Point offset);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
