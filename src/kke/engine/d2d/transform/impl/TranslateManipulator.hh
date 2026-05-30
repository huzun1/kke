#pragma once

#include "kke/appearance/transform/Translation.hh"
#include "kke/engine/d2d/transform/Manipulator.hh"

namespace kke {
class TranslateManipulator : public Manipulator {
	Translation translation;

public:
	TranslateManipulator(Translation const& translation);

	void transform(D2D1::Matrix3x2F& matrix) override;
};
};	// namespace kke
