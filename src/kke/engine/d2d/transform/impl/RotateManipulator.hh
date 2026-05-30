#pragma once

#include "kke/engine/d2d/transform/Manipulator.hh"
#include "kke/appearance/transform/Rotation.hh"

namespace kke {
class RotateManipulator : public Manipulator {
	Rotation rotation;

public:
	RotateManipulator(Rotation const& rotation);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
