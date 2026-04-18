#pragma once

#include "kke/geometry/primitives/Point.hh"

class MatrixManipulator {
public:
	void pushTranslate(kke::Point const& offset);

	void pushScale(kke::Point const& center, kke::Scale const& scale);

	void pushRotate(kke::Point const& center, float angle);

	void popTranslate();

	void popScale();

	void popRotate();

private:
	void popTransform();
};
