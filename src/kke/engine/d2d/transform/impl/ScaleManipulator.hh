#pragma once

#include "kke/appearance/transform/Scaling.hh"
#include "kke/engine/d2d/transform/Manipulator.hh"

namespace kke {
class ScaleManipulator : public Manipulator {
	kke::Point center;
	kke::Scale scale;

  public:
	ScaleManipulator(Scaling const& scale);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
}; // namespace kke
